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
		if (!m_ReceiveAuthenticationPacket())
		{ 
			m_pClient->disconnect();
			std::cin.get();
			return;
		}
		StartSendingPacketStream();
		});
	listenerThread.join();
	std::cout << "Listener thread finished.\n";
}

void oc::ocServer::Create()
{
	if (m_pListener)
	{
		// Something went wrong here. We should never call this function if the listener is set already.
		std::cout << "Tried to create TCP Listener when one already existed.\n";
		std::cout << "Press enter to exit.\n";
		std::cin.get();
		std::exit(-1);
	}

	m_pListener = std::make_unique<sf::TcpListener>();
	if (m_pListener->listen(oc::kPort) != sf::Socket::Status::Done)
	{
		std::cout << "Can't create TCP listener on port " << oc::kPort << "\n";
		return;
	}

	if (m_pClient)
	{
		// Something went wrong here. We should never call this function if the client is set already.
		std::cout << "Tried to create Client when one already existed.\n";
		std::cout << "Press enter to exit.\n";
		std::cin.get();
		std::exit(-1);
	}
	m_pClient = std::make_unique<sf::TcpSocket>();
}

void oc::ocServer::WaitForClient()
{
	// This should never be called when client member variable is empty, but it's safe to do a runtime check.
	if (!m_pClient)
	{
		std::cout << "Tried to wait for a client which was not set.\n";
		std::cout << "Press enter to exit.\n";
		std::cin.get();
		std::exit(-1);
	}

	std::cout << "Waiting for client.\n";
	if (m_pListener->accept(*m_pClient) != sf::Socket::Status::Done)
	{
		std::cout << "Can't create client on port " << oc::kPort << "\n";
		return;
	}

	std::cout << "We have a client! IP: " << m_pClient->getRemoteAddress() << "\n";
}

bool oc::ocServer::m_ReceiveAuthenticationPacket()
{
	auto authenticationPkt = sf::Packet();
	if (m_pClient->receive(authenticationPkt) != sf::Socket::Status::Done)
	{
		std::cout << "Failed at getting authentication packet.\nQuitting.\n";
		return false;
	}
	std::uint32_t major, minor, revision;
	authenticationPkt >> major >> minor >> revision;

	ocVersion version(major, minor, revision);
	if (version.GetVersionString() != Version.GetVersionString())
	{
		std::cout << "Version mismatch!!!\nClient version: " << version.GetVersionString() << "\n";
		std::cout << "Server version: " << Version.GetVersionString() << "\nKicking client.\n";
		m_pClient->disconnect();
		return false;
	}
	std::cout << "Client authentication successful.\n";
	return true;
}

void oc::ocServer::StartSendingPacketStream()
{
	while (true)
	{
		auto pkt = sf::Packet();
		std::string pktData = "Hello, from the server!.";
		pkt << pktData;
		if (m_pClient->send(pkt) != sf::Socket::Status::Done)
		{
			std::cout << "Client disconnected.\nGracefully quitting.\n";
			return;
		}
		std::cout << "Finished sending a packet to the client.\n";
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
	}
}
