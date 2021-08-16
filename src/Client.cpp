#include "Client.h"

void oc::ocClient::Start()
{
	std::thread clientThread([&] {
		Create();
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
	std::cout << "Client thread finished.\n";
}

void oc::ocClient::Create()
{
	m_ServerIP = GetUserIP("Insert server IP\n");
}

void oc::ocClient::ConnectToServer()
{
	auto status = m_pServer->connect(m_ServerIP, oc::kPort);

	if (status != sf::Socket::Status::Done)
	{
		std::cout << "Client can't connect to server.\n";
		std::cin.get();
		return;
	}
	std::cout << "Connected to server successfully.\n";
}

bool oc::ocClient::m_SendAuthenticationPacket()
{
	auto authenticationPkt = sf::Packet();

	authenticationPkt << Version.GetMajor() << Version.GetMinor() << Version.GetRevision();
	if (m_pServer->send(authenticationPkt) != sf::Socket::Status::Done)
	{
		std::cout << "Client authentication FAILED.\n";
		return false;
	}
	std::cout << "Client authentication successful.\n";
	return true;
}

void oc::ocClient::StartReceivingPacketStream()
{
	auto pkt = sf::Packet();
	while (true)
	{
		if (m_pServer->receive(pkt) != sf::Socket::Status::Done)
		{
			m_pServer->disconnect();
			std::cout << "Client lost connection with server.\nQuitting.\n";
			std::cin.get();
			return;
		}
		std::string str;
		pkt >> str;
		std::cout << "I'm a client. Received packet from server. Packet string: " << str << "\n";
		pkt.clear();
	}
}
