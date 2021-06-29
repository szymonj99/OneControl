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
	std::thread listenerThread([&] { Create(); });
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

	std::wcout << L"Waiting for client.\n";

	if (listener->accept(*client) != sf::Socket::Status::Done)
	{
		std::wcout << L"Can't create client on port " << oc::port << L"\n";
		return;
	}

	// c_str() looks ugly but can't convert from string to wstring.
	std::wcout << L"We have a client! IP: " << client->getRemoteAddress().toString().c_str() << L"\n";
	SetClient(client);
	std::wcout << L"We receive client messages here.\n";

	if (!m_ReceiveAuthenticationPacket())
	{
		return;
	}

	while (true)
	{
		sf::Packet pkt = sf::Packet();
		if (GetClient()->receive(pkt) != sf::Socket::Status::Done)
		{
			//auto std::codecvt_utf8_utf32

			//std::cout << U"Testing\n";
			std::wcout << L"Client disconnected.\nGracefully quitting.\n";
			return;
		}
		std::wstring data;
		pkt >> data;
		std::wcout << data << L"\n";
	}
}

void oc::ocServer::WaitForClient()
{

}