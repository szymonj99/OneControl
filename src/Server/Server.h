#pragma once

#define UNICODE 1

#ifdef _WIN32
#include <Windows.h>
#endif

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>

#include <SFML/Network.hpp>
#include <fmt/core.h>
#include <fmt/color.h>

#include "../Helpers/Constants.h"
#include "../Version/Version.h"
#include "../MouseSender/MouseSender.h"
#include "../KeyboardSender/KeyboardSender.h"

namespace oc
{
	class Server : sf::TcpSocket
	{
	private:
		std::unique_ptr<sf::TcpListener> m_pListener = std::make_unique<sf::TcpListener>();
		std::unique_ptr<sf::TcpSocket> m_pClient = std::make_unique<sf::TcpSocket>();
		bool m_ReceiveAuthenticationPacket();

	public:
		void Start();
		void WaitForClient();
		void ServerLoop();
		bool SendPacketToClient(sf::Packet& kPacket);
	};
}
