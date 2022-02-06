#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>

#include <SFML/Network.hpp>

#include "../Helpers/StaticHelpers.h"
#include "../Helpers/Constants.h"
#include "../Version/Version.h"
#include "../MouseReceiver/MouseReceiver.h"
#include "../KeyboardReceiver/KeyboardReceiver.h"

namespace oc
{
	class Client : sf::TcpSocket
	{
	private:
		bool m_SendAuthenticationPacket();

	public:
		void Start();
		void ConnectToServer(const sf::IpAddress& kIPAddress);
		void StartReceivingPacketStream();
	};
}
