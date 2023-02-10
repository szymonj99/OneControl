#include <OneControl/Server.h>

void oc::Server::Start()
{
	std::thread listenerThread([&] {
		WaitForClient();
		ServerLoop();
		});
	listenerThread.join();
	fmt::print("Listener thread finished.\n");
}

void oc::Server::WaitForClient()
{
	// This should never be called when client member variable is empty, but it's safe to do a runtime check.
	if (m_pClient == nullptr)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Tried to wait for a client which was not set.\nPress enter to exit.\n");
		std::cin.get();
		std::exit(-1);
	}

	const auto port = oc::RuntimeGlobals::customPort ? oc::RuntimeGlobals::port : oc::kDefaultPort;
	// Enum class warning
	#pragma warning(suppress: 26812)
	if (m_pListener->listen(port) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Can't listen using TCP listener on port {}\n", port);
		return;
	}

	fmt::print("Waiting for client.\n");
    // TODO: Add in an assert for m_pClient not being nullptr.
	if (m_pListener->accept(*m_pClient) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Can't accept client on port {}\n", port);
		return;
	}

	fmt::print(fmt::fg(fmt::color::green), "We have a client! {}:{}\n", m_pClient->getRemoteAddress().toString(), std::to_string(m_pClient->getRemotePort()));

	if (!m_ReceiveAuthenticationPacket())
	{
		m_pClient->disconnect();
		std::cin.get();
		return;
	}
}

bool oc::Server::m_ReceiveAuthenticationPacket()
{
	auto authenticationPkt = oc::Packet();
	if (m_pClient->receive(authenticationPkt) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Failed at getting authentication packet.\nQuitting.\n");
		return false;
	}
	oc::VersionInt major, minor, revision;
	authenticationPkt >> major >> minor >> revision;

	oc::Version version(major, minor, revision);
	if (version.GetVersionString() != oc::kVersion.GetVersionString())
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "!!!Version mismatch!!!\n");
		fmt::print(stderr, "Client version : {}\n", version.GetVersionString());
		fmt::print(stderr, "Server version : {}\nKicking client.\n", oc::kVersion.GetVersionString());
		m_pClient->disconnect();
		return false;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return true;
}

bool oc::Server::SendPacketToClient(oc::Packet& kPacket)
{
	return m_pClient->send(kPacket) == sf::Socket::Status::Done;
}

void oc::Server::ServerLoop()
{
    // TODO: Make this nicer.
	std::thread mouseThread;
    const auto processMouse = [&]
    {
        const auto mouseInterface = std::make_unique<ol::InputGathererMouse>(true);
        while (oc::RuntimeGlobals::sendToClient)
        {
            oc::Packet pkt {};
            const auto kInput = mouseInterface->GatherInput();
            pkt << kInput;

            if (!this->SendPacketToClient(pkt))
            {
                fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to send packet to client");
                // TODO: Investigate if this needs to be wrapped in a mutex as it can be accessed by more than one thread at a time.
                // Or change it to atomic.
                oc::RuntimeGlobals::sendToClient = false;
            }
        }
    };
    if (oc::RuntimeGlobals::mouseEnabled) { std::thread mouseThread(processMouse); }

	std::thread keyboardThread;
    const auto processKeyboard = [&]
    {
        const auto keyboardInterface = std::make_unique<ol::InputGathererKeyboard>(true);
        while (oc::RuntimeGlobals::sendToClient)
        {
            oc::Packet pkt {};
            const auto kInput = keyboardInterface->GatherInput();
            pkt << kInput;

            if (!this->SendPacketToClient(pkt))
            {
                fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to send packet to client");
                oc::RuntimeGlobals::sendToClient = false;
            }
        }
    };
	if (oc::RuntimeGlobals::keyboardEnabled) { std::thread keyboardThread(processKeyboard); }

    if (oc::RuntimeGlobals::mouseEnabled) { mouseThread.join(); }
    if (oc::RuntimeGlobals::keyboardEnabled) { keyboardThread.join(); }
}
