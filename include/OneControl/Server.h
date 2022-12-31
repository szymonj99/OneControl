#pragma once

#ifdef OS_WINDOWS
#include <Windows.h>
#include <OneLibrary/MessageTimer.h>
#endif

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>

#include <fmt/core.h>
#include <fmt/color.h>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include <OneControl/Packet.h>
#include <OneControl/Constants.h>
#include <OneControl/Version.h>
#include <OneLibrary/MouseSender.h>
#include <OneLibrary/KeyboardSender.h>
#include <OneControl/RuntimeGlobals.h>

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
		bool SendPacketToClient(oc::Packet& kPacket);
	};
}