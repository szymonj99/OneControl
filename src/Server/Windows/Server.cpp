#ifdef _WIN32

#include <SFML/Window/Mouse.hpp>

#include "../Server.h"

void oc::Server::StartSendingPacketStream()
{
	auto mouseInterface = std::make_unique<Mouse>();
	auto pkt = sf::Packet();
	MSG msg;
	while (GetMessage(&msg, oc::Mouse::RawInputMessageWindow, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		pkt << oc::Mouse::RelativeMouseMovement.first << oc::Mouse::RelativeMouseMovement.second;

		// Enum class warning
		#pragma warning(suppress: 26812)
		if (m_pClient->send(pkt) != sf::Socket::Status::Done)
		{
			fmt::print(fmt::fg(fmt::color::red), "Client disconnected.\n");
			fmt::print("Gracefully quitting.\n");
			return;
		}
		pkt.clear();
	}
}

#endif
