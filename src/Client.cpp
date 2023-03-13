#include <OneControl/Client.h>

void oc::Client::Start()
{
	const auto kServerIP = oc::RuntimeGlobals::customServerIP ? oc::RuntimeGlobals::serverIP : oc::GetUserIP("Insert server IP\n");
	if (this->ConnectToServer(kServerIP) != oc::ReturnCode::Success) { return; }
	if (this->AuthenticateToServer() != oc::ReturnCode::Success) { return; }

	const auto kSimulatorMouse = std::make_unique<ol::InputSimulatorMouse>();
	const auto kSimulatorKeyboard = std::make_unique<ol::InputSimulatorKeyboard>();

	oc::Crypto::EncryptorDecryptor<ol::Input> decryptor{};
	while (this->m_bReceiveFromServer)
	{
		oc::Packet pkt{};
		const auto kResult = this->ReceivePacket(pkt);
		if (kResult != oc::ReturnCode::Success)
		{
			this->m_bReceiveFromServer = false;
			this->Disconnect();
			fmt::print(stderr, fmt::fg(fmt::color::red), "Lost connection with server.\n");
			return;
		}

		std::string encryptedInput;
		pkt >> encryptedInput;
		const ol::Input kInput = decryptor.Decrypt(encryptedInput);
		switch (kInput.inputType)
		{
			case ol::eInputType::Mouse:
			{
				kSimulatorMouse->PerformInput(kInput);
				break;
			}
			case ol::eInputType::Keyboard:
			{
				kSimulatorKeyboard->PerformInput(kInput);
				break;
			}
			default:
			{
				fmt::print(stderr, fmt::fg(fmt::color::red), "Uh-oh. We got a packet with incorrect type: {}\n", static_cast<uint16_t>(kInput.inputType));
				break;
			}
		}
	}
}

oc::ReturnCode oc::Client::ConnectToServer(const sf::IpAddress& kIPAddress)
{
	const auto port = oc::RuntimeGlobals::customPort ? oc::RuntimeGlobals::port : oc::kDefaultPort;
	// Enum class warning
	#pragma warning(suppress: 26812)
	if (connect(kIPAddress, port) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Client can't connect to server.\n");
		return oc::ReturnCode::NotAbleToConnectToServer;
	}
	fmt::print(fmt::fg(fmt::color::green), "Connected to server successfully.\n");
	return oc::ReturnCode::Success;
}

oc::ReturnCode oc::Client::m_fHandshake()
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

	const auto kPubKeyResult = this->SendPacket(clientPublicKeyPkt);
	if (kPubKeyResult != oc::ReturnCode::Success)
	{
		std::cerr << "Failed to send public key packet to server." << std::endl;
		return kPubKeyResult;
	}

	// The encrypted AES key will be sent from the server to this, the client.

	// Here, we now will get the encrypted AES key from the server.

	oc::Packet encryptedAESKPkt{};
	const auto kAESKeyResult = this->ReceivePacket(encryptedAESKPkt);
	if (kAESKeyResult != oc::ReturnCode::Success)
	{
		std::cerr << "Failed to receive encrypted AES key from the server." << std::endl;
		return kAESKeyResult;
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

	return m_fSendAuthenticationPacket();
}

oc::ReturnCode oc::Client::m_fSendAuthenticationPacket()
{
	oc::Crypto::EncryptorDecryptor<oc::Version> versionEncryptor{};
	oc::Packet authenticationPkt{};
	authenticationPkt << versionEncryptor.Encrypt(oc::kVersion);

	const auto kResult = this->SendPacket(authenticationPkt);
	if (kResult != oc::ReturnCode::Success)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Client authentication FAILED.\n");
		return kResult;
	}

	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return oc::ReturnCode::Success;
}

oc::ReturnCode oc::Client::AuthenticateToServer()
{
	return this->m_fHandshake();
}

void oc::Client::Disconnect()
{
	this->disconnect();
}

oc::ReturnCode oc::Client::SendPacket(oc::Packet& inPacket)
{
	return (this->send(inPacket) == sf::Socket::Status::Done ? oc::ReturnCode::Success : oc::ReturnCode::FailedSendingPacket);
}

oc::ReturnCode oc::Client::ReceivePacket(oc::Packet& outPacket)
{
	if (this->receive(outPacket) != sf::Socket::Status::Done)
	{
		return oc::ReturnCode::NoPacketReceived;
	}
	return oc::ReturnCode::Success;
}
