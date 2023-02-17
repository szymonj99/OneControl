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

	if (this->m_ReceiveAuthenticationPacket() != oc::ReturnCode::Success)
	{
		this->m_pClient->disconnect();
		return;
	}
}

oc::ReturnCode oc::Server::m_ReceiveAuthenticationPacket()
{
	auto authenticationPkt = oc::Packet();
	if (m_pClient->receive(authenticationPkt) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Failed at getting authentication packet.\nQuitting.\n");
		return oc::ReturnCode::FailedSendingPacket;
	}
	oc::VersionInt major, minor, revision;
	authenticationPkt >> major >> minor >> revision;

	oc::Version version(major, minor, revision);
	if (version.GetVersionString() != oc::kVersion.GetVersionString())
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "!!!Version mismatch!!!\n");
		fmt::print(stderr, "Client version : {}\n", version.GetVersionString());
		fmt::print(stderr, "Server version : {}\nKicking client.\n", oc::kVersion.GetVersionString());
		this->m_pClient->disconnect();
		return oc::ReturnCode::VersionMismatch;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return oc::ReturnCode::Success;
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
