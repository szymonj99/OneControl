#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>

#include <SFML/Network/TcpSocket.hpp>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/base64.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

#include <OneLibrary/InputSimulatorMouse.h>
#include <OneLibrary/InputSimulatorKeyboard.h>
#include <OneControl/Packet.h>
#include <OneControl/HelperMethods.h>
#include <OneControl/Constants.h>
#include <OneControl/Version.h>
#include <OneControl/Input.h>
#include <OneControl/ReturnCode.h>

namespace oc
{
	/**
	 * This class is used when the current machine acts as the client.
	 */
	class Client : sf::TcpSocket
	{
	private:
		oc::ReturnCode m_SendAuthenticationPacket();
		oc::ReturnCode m_Handshake();

	public:
		/**
		 * Starts the client. If an IP has been specified as a command-line argument, it is used. Otherwise prompts the user for input.
		 */
		void Start();
		/**
		 * Reaches out to the server and tries to connect.
		 */
		void ConnectToServer(const sf::IpAddress& kIPAddress);
		/**
		 * Start the message loop. Messages will be sent by the server and input will be performed.
		 */
		void StartReceivingPacketStream();
	};
}
