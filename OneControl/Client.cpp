#include "Client.h"

void oc::ocClient::Start()
{
	std::thread clientThread([&] {
		Create();
		ConnectToServer();
		if (!m_SendAuthenticationPacket())
		{
			m_pServer->disconnect();
			std::wcin.get();
			return;
		}
		StartReceivingPacketStream();
		});
	clientThread.join();
	std::wcout << L"Client thread finished.\n";
}

void oc::ocClient::Create()
{
	m_pServer = std::make_unique<sf::TcpSocket>();
	m_ServerIP = GetUserIP(std::wstring_view(L"Insert server IP\n"));
}

void oc::ocClient::ConnectToServer()
{
	auto status = m_pServer->connect(m_ServerIP.toString(), oc::port);

	if (status != sf::Socket::Status::Done)
	{
		std::wcout << L"Client can't connect to server.\n";
		std::wcin.get();
		return;
	}
	std::wcout << L"Connected to server successfully.\n";
}

bool oc::ocClient::m_SendAuthenticationPacket()
{
	auto authenticationPkt = sf::Packet();

	authenticationPkt << Version.GetMajor() << Version.GetMinor() << Version.GetRevision();
	if (m_pServer->send(authenticationPkt) != sf::Socket::Status::Done)
	{
		std::wcout << L"Client authentication FAILED.\n";
		return false;
	}
	std::wcout << L"Client authentication successful.\n";
	return true;
}

void oc::ocClient::StartReceivingPacketStream()
{
	while (true)
	{
		auto pkt = sf::Packet();
		if (!m_pServer->receive(pkt) != sf::Socket::Status::Done)
		{
			m_pServer->disconnect();
			std::wcout << L"Client lost connection with server.\nQuitting.\n";
			std::wcin.get();
			return;
		}
		std::wstring str;
		pkt >> str;
		std::wcout << L"I'm a client. Received packet from server. Packet wstring: " << str << L"\n";
	}
}
