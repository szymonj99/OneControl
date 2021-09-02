#include "Server.h"

inline void oc::ocServer::SetClient(std::unique_ptr<sf::TcpSocket>& client)
{
	m_pClient = std::move(client);
}

inline std::unique_ptr<sf::TcpSocket>& oc::ocServer::GetClient()
{
	return m_pClient;
}

void oc::ocServer::Start()
{
	std::thread listenerThread([&] { 
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

	if (m_pListener->listen(oc::kPort) != sf::Socket::Status::Done)
	{
		std::cout << "Can't listen using TCP listener on port " << oc::kPort << "\n";
		return;
	}

	std::cout << "Waiting for client.\n";
	if (m_pListener->accept(*m_pClient) != sf::Socket::Status::Done)
	{
		std::cout << "Can't accept client on port " << oc::kPort << "\n";
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
		std::cout << "!!!Version mismatch!!!\n";
		std::cout << "Client version : " << version.GetVersionString() << "\n";
		std::cout << "Server version: " << Version.GetVersionString() << "\nKicking client.\n";
		m_pClient->disconnect();
		return false;
	}
	std::cout << "Client authentication successful.\n";
	return true;
}
