#include "Client.h"

void oc::Client::Start()
{
	std::thread clientThread([&] {
		const auto kServerIP = GetUserIP("Insert server IP\n");
		ConnectToServer(kServerIP);
		StartReceivingPacketStream();
		});
	clientThread.join();
	fmt::print("Client thread finished.\n");
}

void oc::Client::ConnectToServer(const sf::IpAddress& kIPAddress)
{
	
	// Enum class warning
	#pragma warning(suppress: 26812)
	if (connect(kIPAddress, oc::kPort) != sf::Socket::Status::Done)
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
	auto authenticationPkt = sf::Packet();
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
	auto pkt = sf::Packet();

	auto mouseInterface = std::make_unique<oc::MouseReceiver>();
	oc::MousePair mousePrevious = { 0, 0 };
	oc::MousePair mouseCurrent = { 0, 0 };
	oc::MousePair mouseToMove;

	auto keyboardInterface = std::make_unique<oc::KeyboardReceiver>();
	oc::KeyboardPair keyboardPair = { 0,0 };

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
		
		oc::InputInt packetType;
		pkt >> packetType;
		const auto type = static_cast<oc::eInputType>(packetType);

		switch (type)
		{
		// The first time we receive a mouse packet, previous = {0,0} and current = {x, y} where x and y can be big, e.g. x = 800, y = 600
		// This results in a big movement spike.
		case oc::eInputType::Mouse:
			pkt >> mouseCurrent.first >> mouseCurrent.second;
			mouseToMove = { mouseCurrent.first - mousePrevious.first, mouseCurrent.second - mousePrevious.second };
			mousePrevious = mouseCurrent;
			mouseInterface->MoveMouseRelative(mouseToMove.first, mouseToMove.second);
			break;
		case oc::eInputType::Keyboard:
			pkt >> keyboardPair.first >> keyboardPair.second;
			keyboardInterface->KeyPress(keyboardPair.first, keyboardPair.second);
			break;
		default:
			break;
		}
		pkt.clear();
	}
}
