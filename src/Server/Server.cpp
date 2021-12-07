#include "Server.h"

inline void oc::Server::SetClient(std::unique_ptr<sf::TcpSocket>& client)
{
	m_pClient = std::move(client);
}

inline std::unique_ptr<sf::TcpSocket>& oc::Server::GetClient()
{
	return m_pClient;
}

void oc::Server::Start()
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
	fmt::print("Listener thread finished.\n");
}

void oc::Server::WaitForClient()
{
	// This should never be called when client member variable is empty, but it's safe to do a runtime check.
	if (!m_pClient)
	{
		fmt::print(fmt::fg(fmt::color::red), "Tried to wait for a client which was not set.\nPress enter to exit.\n");
		std::cin.get();
		std::exit(-1);
	}

	if (m_pListener->listen(oc::kPort) != sf::Socket::Status::Done)
	{
		fmt::print(fmt::fg(fmt::color::red), "Can't listen using TCP listener on port {}\n", oc::kPort);
		return;
	}

	fmt::print("Waiting for client.\n");
	if (m_pListener->accept(*m_pClient) != sf::Socket::Status::Done)
	{
		fmt::print(fmt::fg(fmt::color::red), "Can't accept client on port {}\n", oc::kPort);
		return;
	}

	fmt::print(fmt::fg(fmt::color::green), "We have a client! IP: {}\n", m_pClient->getRemoteAddress().toString());
}

bool oc::Server::m_ReceiveAuthenticationPacket()
{
	auto authenticationPkt = sf::Packet();
	if (m_pClient->receive(authenticationPkt) != sf::Socket::Status::Done)
	{
		fmt::print(fmt::fg(fmt::color::red), "Failed at getting authentication packet.\nQuitting.\n");
		return false;
	}
	std::uint32_t major, minor, revision;
	authenticationPkt >> major >> minor >> revision;

	Version version(major, minor, revision);
	if (version.GetVersionString() != oc::kVersion.GetVersionString())
	{
		fmt::print(fmt::fg(fmt::color::red), "!!!Version mismatch!!!\n");
		fmt::print("Client version : {}\n", version.GetVersionString());
		fmt::print("Server version : {}\nKicking client.\n", oc::kVersion.GetVersionString());
		m_pClient->disconnect();
		return false;
	}
	fmt::print(fmt::fg(fmt::color::green), "Client authentication successful.\n");
	return true;
}
