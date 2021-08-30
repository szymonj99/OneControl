#ifdef __APPLE__

#include "../Server.h"

void oc::ocServer::StartSendingPacketStream()
{
	auto mouseInterface = std::make_unique<Mouse>();
	auto pkt = sf::Packet();
	std::string pktData = "Hello, from the server!.";
	pkt << pktData;
	while (true)
	{
		if (m_pClient->send(pkt) != sf::Socket::Status::Done)
		{
			std::cout << "Client disconnected.\nGracefully quitting.\n";
			return;
		}
		std::cout << "Finished sending a packet to the client.\n";
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
	}
}

#endif
