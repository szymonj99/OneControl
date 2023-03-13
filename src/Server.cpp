#include <OneControl/Server.h>

// TODO: Refactor this more.
void oc::Server::Start()
{
	if (this->WaitForClient() != oc::ReturnCode::Success) { return; }
	if (this->AuthenticateClient() != oc::ReturnCode::Success) { return; }

	// TODO: Validate the client here by asking the user to input a number that is present on the client machine to authenticate both machines.

	const auto kGathererKeyboard = std::make_unique<ol::InputGathererKeyboard>(true);
	const auto kGathererMouse = std::make_unique<ol::InputGathererMouse>(true);

	std::jthread mouseThread{[&](const std::stop_token& stopToken)
	{
		std::stop_callback stopCallback{stopToken, [&]{ kGathererMouse->Shutdown(); this->m_bufInputs.Interrupt(); this->m_bSendToClient = false; }};

		try
		{
			while (this->m_bSendToClient)
			{
				const auto kInput = kGathererMouse->GatherInput();
				this->m_bufInputs.Add(kInput);
			}
		}
		catch (const ol::InterruptException& e)
		{
			return;
		}
	}};

	std::jthread keyboardThread{[&](const std::stop_token& stopToken)
	{
		std::stop_callback stopCallback{stopToken, [&]{ kGathererKeyboard->Shutdown(); this->m_bufInputs.Interrupt(); this->m_bSendToClient = false; }};

		try
		{
			while (this->m_bSendToClient)
			{
				const auto kInput = kGathererKeyboard->GatherInput();
				this->m_bufInputs.Add(kInput);
			}
		}
		catch (const ol::InterruptException& e)
		{
			return;
		}
	}};

	oc::Crypto::EncryptorDecryptor<ol::Input> inputEncryptor{};

	try
	{
		while (this->m_bSendToClient)
		{
			oc::Packet pkt{};
			const auto kInput = this->GetNextInput();

			// Kill switch
			if (kInput.keyboard.key == ol::eKeyCode::End)
			{
				this->Disconnect();
				this->m_bufInputs.Interrupt();
				return;
			}

			if (kInput.keyboard.key == ol::eKeyCode::Home)
			{
				if (kInput.eventType == ol::eEventType::KBKeyDown)
				{
					kGathererMouse->Toggle();
					kGathererKeyboard->Toggle();
				}
				continue;
			}

			pkt << inputEncryptor.Encrypt(kInput);
			if (this->SendPacket(pkt) != oc::ReturnCode::Success)
			{
				fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to send packet to client\n");
				return;
			}
		}
	}
	catch (const ol::InterruptException& e)
	{
		return;
	}
}

ol::Input oc::Server::GetNextInput()
{
	return this->m_bufInputs.Get();
}

oc::ReturnCode oc::Server::WaitForClient()
{
	const auto port = oc::RuntimeGlobals::customPort ? oc::RuntimeGlobals::port : oc::kDefaultPort;
	// Enum class warning
	#pragma warning(suppress: 26812)
	// TODO: Since we wrapped other sfml calls like Disconnect, should we wrap this one too?
	if (this->m_pListener->listen(port) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Can't listen using TCP listener on port {}\n", port);
		return oc::ReturnCode::NotAbleToBindOnPort;
	}

	fmt::print("Waiting for client.\n");
	// TODO: Since we wrapped other sfml calls like Disconnect, should we wrap this one too?
	if (this->m_pListener->accept(*m_pClient) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Can't accept client on port {}\n", port);
		return oc::ReturnCode::NotAbleToAcceptClient;
	}

	fmt::print(fmt::fg(fmt::color::green), "We have a client! {}:{}\n", this->m_pClient->getRemoteAddress().toString(), std::to_string(this->m_pClient->getRemotePort()));
	return oc::ReturnCode::Success;
}

oc::ReturnCode oc::Server::AuthenticateClient()
{
	return this->m_fHandshake();
}

oc::ReturnCode oc::Server::m_fReceiveAuthenticationPacket()
{
	oc::Packet authenticationPkt{};
	const auto kResult = this->ReceivePacket(authenticationPkt);
	if (kResult != oc::ReturnCode::Success)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Failed at getting authentication packet.\nQuitting.\n");
		return kResult;
	}

	oc::Crypto::EncryptorDecryptor<oc::Version> versionDecryptor{};

	std::string clientVersionStr;
	authenticationPkt >> clientVersionStr;
	oc::Version clientVersion = versionDecryptor.Decrypt(clientVersionStr);
	if (oc::kVersion != clientVersion)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "!!!Version mismatch!!!\n");
		fmt::print(stderr, "Client version : {}\n", oc::VersionToString(clientVersion));
		fmt::print(stderr, "Server version : {}\nKicking client.\n", oc::VersionToString(oc::kVersion));
		this->Disconnect();
		return oc::ReturnCode::VersionMismatch;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return oc::ReturnCode::Success;
}

oc::ReturnCode oc::Server::m_fHandshake()
{
	std::cout << "Performing OneControl-specific Handshake with Client." << std::endl;

	// The client will reach out to us, the server, with it's RSA public key.
	// Let's not add an option to disable encryption for now. That may be potentially done in the future.
	oc::Packet clientPublicKeyPkt{};
    const auto kPubKeyResult = this->ReceivePacket(clientPublicKeyPkt);
    if (kPubKeyResult != oc::ReturnCode::Success)
    {
        std::cerr << "Failed to receive public key packet from client." << std::endl;
        return kPubKeyResult;
    }

	// This server doesn't actually need to create its own RSA keys, as we get it from the client, and encrypt our AES key with it.
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

	const auto kAESResult = this->SendPacket(encryptedAESPkt);
	if (kAESResult != oc::ReturnCode::Success)
	{
		std::cerr << "Failed to send encrypted AES key to client." << std::endl;
		return kAESResult;
	}

	// Here we have an agreed-upon AES key that was sent securely.
	oc::Crypto::aesEncryption = { aesK, aesK.size() };
	oc::Crypto::cbcEncryption = { oc::Crypto::aesEncryption, iv };
	oc::Crypto::aesDecryption = { aesK, aesK.size() };
	oc::Crypto::cbcDecryption = { oc::Crypto::aesDecryption, iv };

	std::cout << "Established Hybrid Encryption Successfully." << std::endl;

	return this->m_fReceiveAuthenticationPacket();
}

oc::ReturnCode oc::Server::SendPacket(oc::Packet& inPacket)
{
	return (this->m_pClient->send(inPacket) == sf::Socket::Status::Done ? oc::ReturnCode::Success : oc::ReturnCode::FailedSendingPacket);
}

void oc::Server::Disconnect()
{
    this->m_pClient->disconnect();
}

oc::ReturnCode oc::Server::ReceivePacket(oc::Packet& packet)
{
    if (this->m_pClient->receive(packet) != sf::Socket::Status::Done)
    {
        return oc::ReturnCode::NoPacketReceived;
    }
    return oc::ReturnCode::Success;
}

