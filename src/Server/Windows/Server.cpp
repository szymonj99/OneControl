#ifdef _WIN32

#include "../Server.h"

void oc::ocServer::StartSendingPacketStream()
{
	auto mouseInterface = std::make_unique<Mouse>();
	auto pkt = sf::Packet();

	MSG msg;
	while (GetMessage(&msg, oc::Mouse::RawInputMessageWindow, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		pkt << oc::Mouse::RelativeMouseMovement.first << oc::Mouse::RelativeMouseMovement.second;
		if (m_pClient->send(pkt) != sf::Socket::Status::Done)
		{
			std::cout << "Client disconnected.\nGracefully quitting.\n";
			return;
		}
		pkt.clear();
	}
}

#endif
