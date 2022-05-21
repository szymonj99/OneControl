#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>

#include <SFML/Network/TcpSocket.hpp>

#include <OneControl/Packet/Packet.h>
#include <OneControl/HelperMethods.h>
#include <OneControl/Constants.h>
#include <OneControl/Version/Version.h>
#include <OneControl/Receiver/Mouse/MouseReceiver.h>
#include <OneControl/Receiver/Keyboard/KeyboardReceiver.h>

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
