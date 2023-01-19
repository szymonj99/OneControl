#include <OneControl/Client.h>

void oc::Client::Start()
{
	std::thread clientThread([&] {
		const auto kServerIP = oc::RuntimeGlobals::customServerIP ? oc::RuntimeGlobals::serverIP : GetUserIP("Insert server IP\n");
		ConnectToServer(kServerIP);
		StartReceivingPacketStream();
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
		std::cin.get();
		return;
	}
	fmt::print(fmt::fg(fmt::color::green), "Connected to server successfully.\n");

	if (!m_SendAuthenticationPacket())
	{
		disconnect();
		std::cin.get();
		return;
	}
}

bool oc::Client::m_SendAuthenticationPacket()
{
	auto authenticationPkt = oc::Packet();
	authenticationPkt << oc::kVersion.GetMajor() << oc::kVersion.GetMinor() << oc::kVersion.GetRevision();
	if (send(authenticationPkt) != sf::Socket::Status::Done)
	{
		fmt::print(stderr, fmt::fg(fmt::color::red), "Client authentication FAILED.\n");
		return false;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return true;
}

void oc::Client::StartReceivingPacketStream()
{
	auto pkt = oc::Packet();
    // TODO: When adding in parameter parsing or compile options, add in an option/toggle to allow only mouse or keyboard
    // That will potentially allow some users to compile with potentially less dependencies if they would like to.
    auto mouseSimulator = std::make_unique<ol::InputSimulatorMouse>();
    auto keyboardSimulator = std::make_unique<ol::InputSimulatorKeyboard>();

	while (true)
	{
		if (receive(pkt) != sf::Socket::Status::Done)
		{
			disconnect();
			fmt::print(stderr, fmt::fg(fmt::color::red), "Client lost connection with server.\n");
			fmt::print(stderr, "Quitting.\n");
			std::cin.get();
			return;
		}
		
		ol::Input input{};
		pkt >> input;

        switch (input.inputType)
        {
            case ol::eInputType::Mouse:
                mouseSimulator->PerformInput(input);
                break;
            case ol::eInputType::Keyboard:
                keyboardSimulator->PerformInput(input);
                break;
            default:
                // TODO: Make the static_cast a bit nicer.
                fmt::print(stderr, fmt::fg(fmt::color::red), "Uh-oh. We got a packet with incorrect type: {}\n", static_cast<uint8_t>(input.inputType));
                break;
        }

		pkt.clear();
	}
}
