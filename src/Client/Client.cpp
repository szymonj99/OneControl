#include "Client.h"

void oc::Client::Start()
{
	std::thread clientThread([&] {
		ConnectToServer();
		if (!m_SendAuthenticationPacket())
		{
			m_pServer->disconnect();
			std::cin.get();
			return;
		}
		StartReceivingPacketStream();
		});
	clientThread.join();
	fmt::print("Client thread finished.\n");
}

void oc::Client::ConnectToServer()
{
	m_ServerIP = GetUserIP("Insert server IP\n");

	// Enum class warning
	#pragma warning(suppress: 26812)
	const auto status = m_pServer->connect(m_ServerIP, oc::kPort);
	if (status != sf::Socket::Status::Done)
	{
		fmt::print(fmt::fg(fmt::color::red), "Client can't connect to server.\n");
		std::cin.get();
		return;
	}
	fmt::print(fmt::fg(fmt::color::green), "Connected to server successfully.\n");
}

bool oc::Client::m_SendAuthenticationPacket()
{
	auto authenticationPkt = sf::Packet();

	authenticationPkt << oc::kVersion.GetMajor() << oc::kVersion.GetMinor() << oc::kVersion.GetRevision();
	if (m_pServer->send(authenticationPkt) != sf::Socket::Status::Done)
	{
		fmt::print(fmt::fg(fmt::color::red), "Client authentication FAILED.\n");
		return false;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return true;
}

void oc::Client::StartReceivingPacketStream()
{
	auto mouseInterface = std::make_unique<Mouse>();
	auto pkt = sf::Packet();
	std::pair<int32_t, int32_t> relativeMouseMovement = { 0, 0 };
	while (true)
	{
		if (m_pServer->receive(pkt) != sf::Socket::Status::Done)
		{
			m_pServer->disconnect();
			fmt::print(fmt::fg(fmt::color::red), "Client lost connection with server.\n");
			fmt::print("Quitting.\n");
			std::cin.get();
			return;
		}
		
		pkt >> relativeMouseMovement.first >> relativeMouseMovement.second;
		fmt::print("{} : {}\n", relativeMouseMovement.first, relativeMouseMovement.second);
		mouseInterface->MoveMouseRelative(relativeMouseMovement.first, relativeMouseMovement.second);
		pkt.clear();
	}
}
