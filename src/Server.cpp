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

	CryptoPP::RSA::PublicKey clientPubK;

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey privK;
	constexpr uint32_t kAESKeySize = 2048;
	privK.GenerateRandomWithKeySize(rng, kAESKeySize);
	CryptoPP::RSA::PublicKey pubK(privK);

	std::string clientPubKStr;
	clientPublicKeyPkt >> clientPubKStr;

	CryptoPP::StringSource clientPubKSource(clientPubKStr, true);
	clientPubK.Load(clientPubKSource);

	// Here we have a packet with some info inside.
	// What do we want this info to be actually?
	// AES Key
	CryptoPP::SecByteBlock aesK[CryptoPP::AES::DEFAULT_KEYLENGTH];
	rng.GenerateBlock(aesK->BytePtr(), sizeof(aesK));

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
