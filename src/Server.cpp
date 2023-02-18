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
	CryptoPP::AutoSeededRandomPool rng;

	std::string clientPubKStr;
	clientPublicKeyPkt >> clientPubKStr;

	CryptoPP::StringSource clientPubKSource(clientPubKStr, true);
	CryptoPP::RSA::PublicKey clientPubK;
	clientPubK.Load(clientPubKSource);

	// AES Key
	CryptoPP::SecByteBlock aesK(CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
	rng.GenerateBlock(aesK, aesK.size());
	rng.GenerateBlock(iv, iv.size());

	// Now that this server has the client's public key, we can encrypt the AES key using the Client RSA Public Key.
	CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(clientPubK);
	std::string encryptedAESK;
	CryptoPP::StringSource encryptedAESKSource(aesK, aesK.size(), true, new CryptoPP::PK_EncryptorFilter(rng, encryptor, new CryptoPP::StringSink(encryptedAESK)));

	std::string encryptedIV;
	CryptoPP::StringSource encryptedIVSource(iv, iv.size(), true, new CryptoPP::PK_EncryptorFilter(rng, encryptor, new CryptoPP::StringSink(encryptedIV)));

	oc::Packet encryptedAESPkt{};
	encryptedAESPkt << encryptedAESK << encryptedIV;

	if (this->m_pClient->send(encryptedAESPkt) != sf::Socket::Status::Done)
	{
		std::cerr << "Failed to send encrypted AES key to client." << std::endl;
		return oc::ReturnCode::FailedSendingPacket;
	}

	// Here we have an agreed-upon AES key that was sent securely.
	CryptoPP::AES::Encryption aesEncryption(aesK, aesK.size());
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	CryptoPP::AES::Decryption aesDecryption(aesK, aesK.size());
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	oc::Crypto::aesEncryption = aesEncryption;
	oc::Crypto::aesDecryption = aesDecryption;
	oc::Crypto::cbcEncryption = cbcEncryption;
	oc::Crypto::cbcDecryption = cbcDecryption;

	// TODO: Figure out a way to send a new IV between the server and the client.

	std::cout << "Established Hybrid Encryption Successfully." << std::endl;

	return this->m_ReceiveAuthenticationPacket();
}

void oc::Server::ServerLoop()
{
    // TODO: Make this nicer.
    const auto processMouse = [&]
    {
        const auto mouseInterface = std::make_unique<ol::InputGathererMouse>(true);
        while (oc::RuntimeGlobals::sendToClient)
        {
            oc::Packet pkt {};
            const auto kInput = mouseInterface->GatherInput();
            pkt << kInput;

            if (this->m_pClient->send(pkt) != sf::Socket::Status::Done)
            {
                fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to send packet to client");
                // TODO: Investigate if this needs to be wrapped in a mutex as it can be accessed by more than one thread at a time.
                // Or change it to atomic.
                oc::RuntimeGlobals::sendToClient = false;
            }
        }
    };
    std::thread mouseThread(processMouse);

    const auto processKeyboard = [&]
    {
        const auto keyboardInterface = std::make_unique<ol::InputGathererKeyboard>(true);
        while (oc::RuntimeGlobals::sendToClient)
        {
            oc::Packet pkt {};
            const auto kInput = keyboardInterface->GatherInput();
            pkt << kInput;

            if (this->m_pClient->send(pkt) != sf::Socket::Status::Done)
            {
                fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to send packet to client");
                oc::RuntimeGlobals::sendToClient = false;
            }
        }
    };
	std::thread keyboardThread(processKeyboard);

    mouseThread.join();
    keyboardThread.join();
}
