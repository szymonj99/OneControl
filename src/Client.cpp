#include <OneControl/Client.h>

void oc::Client::Start()
{
	std::thread clientThread([&] {
		const auto kServerIP = oc::RuntimeGlobals::customServerIP ? oc::RuntimeGlobals::serverIP : GetUserIP("Insert server IP\n");
		ConnectToServer(kServerIP);

		if (this->m_Handshake() != oc::ReturnCode::Success)
		{
			std::cerr << "Failed to perform handshake with the server." << std::endl;
			this->disconnect();
			return;
		}

		ClientLoop();
		});
	clientThread.join();
	fmt::print("Client thread finished.\n");
}

void oc::Client::ConnectToServer(const sf::IpAddress& kIPAddress)
{
	const auto port = oc::RuntimeGlobals::customPort ? oc::RuntimeGlobals::port : oc::kDefaultPort;
	// Enum class warning
	#pragma warning(suppress: 26812)
	if (connect(kIPAddress, port) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Client can't connect to server.\n");
		return;
	}
	fmt::print(fmt::fg(fmt::color::green), "Connected to server successfully.\n");
}

oc::ReturnCode oc::Client::m_Handshake()
{
	std::cout << "Performing OneControl-specific Handshake with Server." << std::endl;

	CryptoPP::RSA::PrivateKey privK;
	constexpr uint32_t kAESKeySize = 2048;
	privK.GenerateRandomWithKeySize(oc::Crypto::rng, kAESKeySize);
	CryptoPP::RSA::PublicKey pubK(privK);

	// Let's not add an option to disable encryption for now. That may be potentially done in the future.
	oc::Packet clientPublicKeyPkt{};
	std::string pubKStr;
	CryptoPP::StringSink pubKSink(pubKStr);
	pubK.Save(pubKSink);
	clientPublicKeyPkt << pubKStr;

	// TODO: If we want to output more debug info, we can print out some of the parameters of the keys used, or base64 them.

	if (this->send(clientPublicKeyPkt) != sf::Socket::Status::Done)
	{
		std::cerr << "Failed to send public key packet to server." << std::endl;
		return oc::ReturnCode::FailedSendingPacket;
	}

	// The encrypted AES key will be sent from the server to this, the client.

	// Here, we now will get the encrypted AES key from the server.

	oc::Packet encryptedAESKPkt{};
	if (this->receive(encryptedAESKPkt) != sf::Socket::Status::Done)
	{
		std::cerr << "Failed to receive encrypted AES key from the server." << std::endl;
		return oc::ReturnCode::NoPacketReceived;
	}

	std::string encryptedAESK;
	std::string encryptedIV;
	encryptedAESKPkt >> encryptedAESK >> encryptedIV;

	// Decrypt the AES key.

	CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(privK);
	CryptoPP::SecByteBlock aesK(CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::StringSource decryptedAESKSource(encryptedAESK, true, new CryptoPP::PK_DecryptorFilter(oc::Crypto::rng, decryptor, new CryptoPP::ArraySink(aesK, aesK.size())));
	CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
	CryptoPP::StringSource decryptedIVSource(encryptedIV, true, new CryptoPP::PK_DecryptorFilter(oc::Crypto::rng, decryptor, new CryptoPP::ArraySink(iv, iv.size())));

	// Here we have an agreed-upon AES key that was sent securely as well as the initialisation vector.

	oc::Crypto::aesEncryption = { aesK, aesK.size() };
	oc::Crypto::aesDecryption = { aesK, aesK.size() };
	oc::Crypto::cbcEncryption = { oc::Crypto::aesEncryption, iv };
	oc::Crypto::cbcDecryption = { oc::Crypto::aesDecryption, iv };

	std::cout << "Established Hybrid Encryption Successfully." << std::endl;

	return m_SendAuthenticationPacket();
}

oc::ReturnCode oc::Client::m_SendAuthenticationPacket()
{
	oc::Packet authenticationPkt{};
	authenticationPkt << oc::kVersion;
	if (this->send(authenticationPkt) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Client authentication FAILED.\n");
		return oc::ReturnCode::FailedSendingPacket;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return oc::ReturnCode::Success;
}

void oc::Client::ClientLoop()
{
	std::thread receiver([&]
	{
		oc::Crypto::EncryptorDecryptor<ol::Input> decryptor{};
		while (oc::RuntimeGlobals::receiveFromServer)
		{
			oc::Packet pkt{};
			if (this->receive(pkt) != sf::Socket::Status::Done)
			{
				this->disconnect();
				fmt::print(stderr, fmt::fg(fmt::color::red), "Client lost connection with server.\n");
				fmt::print(stderr, "Quitting.\n");
				return;
			}
			std::string encryptedInput;
			pkt >> encryptedInput;
			const ol::Input kInput = decryptor.Decrypt(encryptedInput);
			this->m_bufInputs.Add(kInput);
		}
	});

    // TODO: When adding in parameter parsing or compile options, add in an option/toggle to allow only mouse or keyboard
    // That will potentially allow some users to compile with potentially less dependencies if they would like to.
    auto mouseSimulator = std::make_unique<ol::InputSimulatorMouse>();
    auto keyboardSimulator = std::make_unique<ol::InputSimulatorKeyboard>();

	while (oc::RuntimeGlobals::receiveFromServer)
	{
		const auto kInput = this->m_bufInputs.Get();
		switch (kInput.inputType)
		{
		case ol::eInputType::Mouse:
			mouseSimulator->PerformInput(kInput);
			break;
		case ol::eInputType::Keyboard:
			keyboardSimulator->PerformInput(kInput);
			break;
		default:
			// TODO: Make the static_cast a bit nicer.
			fmt::print(stderr, fmt::fg(fmt::color::red), "Uh-oh. We got a packet with incorrect type: {}\n", static_cast<uint32_t>(kInput.inputType));
			break;
		}
	}

	receiver.join();
}
