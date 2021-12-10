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
#include "../Mouse/Mouse.h"
#include "../Keyboard/Keyboard.h"

namespace oc
{
	class Server
	{
	private:
		std::unique_ptr<sf::TcpSocket> m_pClient = std::make_unique<sf::TcpSocket>();
		std::unique_ptr<sf::TcpListener> m_pListener = std::make_unique<sf::TcpListener>();

		bool m_ReceiveAuthenticationPacket();

	public:
		void Start();
		void SetClient(std::unique_ptr<sf::TcpSocket>& client);
		std::unique_ptr<sf::TcpSocket>& GetClient();
		void WaitForClient();
		bool SendPacket(sf::Packet& packet);
		void ServerLoop();
	};
}
