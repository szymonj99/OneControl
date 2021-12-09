#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <SFML/Network.hpp>

#include "../Helpers/Constants.h"
#include "../Version/Version.h"
#include "../Program/OneControl.h"
#include "../Mouse/Mouse.h"
#include "../Keyboard/Keyboard.h"

namespace oc
{
	class Client
	{
	private:
		std::unique_ptr<sf::TcpSocket> m_pServer = std::make_unique<sf::TcpSocket>();
		sf::IpAddress m_ServerIP = sf::IpAddress::None;

		bool m_SendAuthenticationPacket();

	public:
		void Start();
		void ConnectToServer();
		void StartReceivingPacketStream();
	};
}
