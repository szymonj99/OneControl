#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <SFML/Network.hpp>

#include "../Helpers/Constants.h"
#include "../Version/Version.h"
#include "../Mouse/Mouse.h"

namespace oc
{
	class ocServer
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
		void StartSendingPacketStream();
	};
}
