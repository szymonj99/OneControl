#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <SFML/Network.hpp>

#include "Constants.h"
#include "Version.h"
#include "OneControl.h"

namespace oc
{
	class ocClient
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
