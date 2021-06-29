#include "Server.h"

void oc::ocServer::SetClient(std::unique_ptr<sf::TcpSocket>& client)
{
	m_pClient = std::move(client);
}

std::unique_ptr<sf::TcpSocket>& oc::ocServer::GetClient()
{
	return m_pClient;
}

void oc::ocServer::Start()
{
	std::thread listenerThread([&] { 
		Create();
		WaitForClient();
		if (!m_ReceiveAuthenticationPacket()){ return; }
		StartSendingPacketStream();
		});
	listenerThread.join();
	std::wcout << L"Listener thread finished.\n";
}

void oc::ocServer::Create()
{
	if (m_pListener)
	{
		// Something went wrong here. We should never call this function if the listener is set already.
		std::wcout << L"Tried to create TCP Listener when one already existed.\n";
		std::wcout << L"Press enter to exit.\n";
		std::wcin.get();
		std::exit(-1);
	}

	m_pListener = std::make_unique<sf::TcpListener>();
	if (m_pListener->listen(oc::port) != sf::Socket::Status::Done)
	{
		std::wcout << L"Can't create TCP listener on port " << oc::port << L"\n";
		return;
	}

	if (m_pClient)
	{
		// Something went wrong here. We should never call this function if the client is set already.
		std::wcout << L"Tried to create Client when one already existed.\n";
		std::wcout << L"Press enter to exit.\n";
		std::wcin.get();
		std::exit(-1);
	}
	m_pClient = std::make_unique<sf::TcpSocket>();
}

void oc::ocServer::WaitForClient()
{
	// This should never be called when client member variable is empty, but it's safe to do a runtime check.
	if (!m_pClient)
	{
		std::wcout << L"Tried to wait for a client which was not set.\n";
		std::wcout << L"Press enter to exit.\n";
		std::wcin.get();
		std::exit(-1);
	}

	std::wcout << L"Waiting for client.\n";
	if (m_pListener->accept(*m_pClient) != sf::Socket::Status::Done)
	{
		std::wcout << L"Can't create client on port " << oc::port << L"\n";
		return;
	}

	// c_str() looks ugly but can't convert from string to wstring.
	std::wcout << L"We have a client! IP: " << m_pClient->getRemoteAddress().toString().c_str() << L"\n";
}

bool oc::ocServer::m_ReceiveAuthenticationPacket()
{
	auto authenticationPkt = sf::Packet();
	if (m_pClient->receive(authenticationPkt) != sf::Socket::Status::Done)
	{
		std::wcout << L"Failed at getting authentication packet.\nQuitting.\n";
		return false;
	}
	std::uint32_t major, minor, revision;
	authenticationPkt >> major >> minor >> revision;

	ocVersion version(major, minor, revision);
	if (version.GetVersionStringView() != Version.GetVersionStringView())
	{
		std::wcout << L"Version mismatch!!!\nClient version: " + std::wstring(version.GetVersionStringView()) + L"\n";
		std::wcout << L"Server version: " + std::wstring(Version.GetVersionStringView()) + L"\nKicking client.\n";
		m_pClient->disconnect();
		return false;
	}
	std::wcout << "Client authentication successful.\n";
	return true;
}

void oc::ocServer::StartSendingPacketStream()
{
	while (true)
	{
		// Rework.
		auto pkt = sf::Packet();
		std::wstring pktData = L"Hello, from the server!.";
		pkt << pktData;
		if (m_pClient->send(pkt) != sf::Socket::Status::Done)
		{
			std::wcout << L"Client disconnected.\nGracefully quitting.\n";
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
	}
}
