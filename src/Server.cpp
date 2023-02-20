#include <OneControl/Server.h>

void oc::Server::Start()
{
	std::thread listenerThread([&] {
		WaitForClient();

		if (this->m_Handshake() != oc::ReturnCode::Success)
		{
			std::cerr << "Failed to perform handshake with the client." << std::endl;
			this->m_pClient->disconnect();
			return;
		}

		ServerLoop();
		});
	listenerThread.join();
	fmt::print("Listener thread finished.\n");
}

void oc::Server::WaitForClient()
{
	const auto port = oc::RuntimeGlobals::customPort ? oc::RuntimeGlobals::port : oc::kDefaultPort;
	// Enum class warning
	#pragma warning(suppress: 26812)
	if (this->m_pListener->listen(port) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Can't listen using TCP listener on port {}\n", port);
		return;
	}

	fmt::print("Waiting for client.\n");
	if (this->m_pListener->accept(*m_pClient) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Can't accept client on port {}\n", port);
		return;
	}

	fmt::print(fmt::fg(fmt::color::green), "We have a client! {}:{}\n", this->m_pClient->getRemoteAddress().toString(), std::to_string(this->m_pClient->getRemotePort()));
}

oc::ReturnCode oc::Server::m_ReceiveAuthenticationPacket()
{
	oc::Packet authenticationPkt{};
	if (m_pClient->receive(authenticationPkt) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Failed at getting authentication packet.\nQuitting.\n");
		return oc::ReturnCode::FailedSendingPacket;
	}
	oc::Version clientVersion;
	authenticationPkt >> clientVersion;
	if (oc::kVersion != clientVersion)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "!!!Version mismatch!!!\n");
		fmt::print(stderr, "Client version : {}\n", oc::VersionToString(clientVersion));
		fmt::print(stderr, "Server version : {}\nKicking client.\n", oc::VersionToString(oc::kVersion));
		this->m_pClient->disconnect();
		return oc::ReturnCode::VersionMismatch;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return oc::ReturnCode::Success;
}

oc::ReturnCode oc::Server::m_Handshake()
{
	std::cout << "Performing OneControl-specific Handshake with Client." << std::endl;

	// The client will reach out to us, the server, with it's RSA public key.
	// Let's not add an option to disable encryption for now. That may be potentially done in the future.
	oc::Packet clientPublicKeyPkt{};
	if (this->m_pClient->receive(clientPublicKeyPkt) != sf::Socket::Status::Done)
	{
		std::cerr << "Failed to receive public key packet from client." << std::endl;
		return oc::ReturnCode::NoPacketReceived;
	}

	// This server doesn't actually need to create it's own RSA keys, as we get it from the client, and encrypt our AES key with it.
	std::string clientPubKStr;
	clientPublicKeyPkt >> clientPubKStr;

	CryptoPP::StringSource clientPubKSource(clientPubKStr, true);
	CryptoPP::RSA::PublicKey clientPubK;
	clientPubK.Load(clientPubKSource);

	// AES Key
	CryptoPP::SecByteBlock aesK(CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
	oc::Crypto::rng.GenerateBlock(aesK, aesK.size());
	oc::Crypto::rng.GenerateBlock(iv, iv.size());

	// Now that this server has the client's public key, we can encrypt the AES key using the Client RSA Public Key.
	CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(clientPubK);
	std::string encryptedAESK;
	CryptoPP::StringSource encryptedAESKSource(aesK, aesK.size(), true, new CryptoPP::PK_EncryptorFilter(oc::Crypto::rng, encryptor, new CryptoPP::StringSink(encryptedAESK)));

	std::string encryptedIV;
	CryptoPP::StringSource encryptedIVSource(iv, iv.size(), true, new CryptoPP::PK_EncryptorFilter(oc::Crypto::rng, encryptor, new CryptoPP::StringSink(encryptedIV)));

	oc::Packet encryptedAESPkt{};
	encryptedAESPkt << encryptedAESK << encryptedIV;

	if (this->m_pClient->send(encryptedAESPkt) != sf::Socket::Status::Done)
	{
		std::cerr << "Failed to send encrypted AES key to client." << std::endl;
		return oc::ReturnCode::FailedSendingPacket;
	}

	// Here we have an agreed-upon AES key that was sent securely.
	oc::Crypto::aesEncryption = { aesK, aesK.size() };
	oc::Crypto::cbcEncryption = { oc::Crypto::aesEncryption, iv };
	oc::Crypto::aesDecryption = { aesK, aesK.size() };
	oc::Crypto::cbcDecryption = { oc::Crypto::aesDecryption, iv };

	std::cout << "Established Hybrid Encryption Successfully." << std::endl;

	return this->m_ReceiveAuthenticationPacket();
}

void oc::Server::ServerLoop()
{
	const auto mouseInterface = std::make_unique<ol::InputGathererMouse>(true);
	const auto keyboardInterface = std::make_unique<ol::InputGathererKeyboard>(true);
	oc::Crypto::EncryptorDecryptor<ol::Input> inputEncryptor{};
	// Buffer the inputs in a single buffer.
	// The reasoning for this is that the mouse and keyboard inputs are stored in separate buffers in OneLibrary
	// Adding them to one buffer makes things more concise but is not necessary.
	// Is this syntax better/worse than having it on multiple lines?
	// TODO: Potentially rename the variable to addToBuffer?

	std::thread mouseThread([&] { while (oc::RuntimeGlobals::sendToClient) { this->m_bufInputs.Add(mouseInterface->GatherInput()); }});
	std::thread keyboardThread([&] { while (oc::RuntimeGlobals::sendToClient) { this->m_bufInputs.Add(keyboardInterface->GatherInput()); }});

	// Send out the buffered inputs here
	while (oc::RuntimeGlobals::sendToClient)
	{
		oc::Packet pkt{};
		const auto kInput = this->m_bufInputs.Get();
		pkt << inputEncryptor.Encrypt(kInput);

		if (this->m_pClient->send(pkt) != sf::Socket::Status::Done)
		{
			fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to send packet to client\n");
			// TODO: Investigate if this needs to be wrapped in a mutex as it can be accessed by more than one thread at a time.
			// Or change it to atomic.
			oc::RuntimeGlobals::sendToClient = false;
			break;
		}
	}

    mouseThread.join();
	keyboardThread.join();
}
