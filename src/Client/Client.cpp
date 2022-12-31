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
		fmt::print(fmt::fg(fmt::color::red), "Client can't connect to server.\n");
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
		fmt::print(fmt::fg(fmt::color::red), "Client authentication FAILED.\n");
		return false;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return true;
}

void oc::Client::StartReceivingPacketStream()
{
	auto pkt = oc::Packet();

	auto mouseInterface = std::make_unique<ol::MouseReceiver>();

	auto keyboardInterface = std::make_unique<ol::KeyboardReceiver>();

	while (true)
	{
		if (receive(pkt) != sf::Socket::Status::Done)
		{
			disconnect();
			fmt::print(fmt::fg(fmt::color::red), "Client lost connection with server.\n");
			fmt::print("Quitting.\n");
			std::cin.get();
			return;
		}
		
		ol::Input input;
		pkt >> input;

		switch (input.type)
		{
		case ol::eInputType::Mouse:
			mouseInterface->MoveMouseRelative(input.mouse.x, input.mouse.y);
			break;
		case ol::eInputType::Keyboard:
			keyboardInterface->KeyPress(input.keyboard.key, input.keyboard.state);
			break;
		case ol::eInputType::Failed:
		case ol::eInputType::Uninitialised:
		case ol::eInputType::HookStopped:
			fmt::print(fmt::fg(fmt::color::red), "Uh-oh. We got a packet with incorrect type: {}\n", static_cast<ol::InputInt>(input.type));
			break;
		case ol::eInputType::KeepAlive:
		default:
			break;
		}
		pkt.clear();
	}
}
