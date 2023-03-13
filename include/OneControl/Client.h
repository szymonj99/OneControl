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
#include <OneLibrary/ThreadsafeQueue.h>
#include <OneLibrary/Input.h>
#include <OneControl/Packet.h>
#include <OneControl/HelperMethods.h>
#include <OneControl/Constants.h>
#include <OneControl/Version.h>
#include <OneControl/ReturnCode.h>
#include <OneControl/Crypto.h>

namespace oc
{
	/**
	 * This class is used when the current machine acts as the client.
	 */
	class Client : sf::TcpSocket
	{
	private:
		// I have decided that the client does not need a buffer.
		// A buffer would be a nice-to-have for when the network connection is drastically faster than the ability for the client to simulate the inputs.
		// This will likely never occur, as local API calls for input emulation are faster than a network connection;
		// Therefore, but the time a new input packet comes in, the client has performed the previous input already.
		oc::ReturnCode m_fSendAuthenticationPacket();
		oc::ReturnCode m_fHandshake();
		std::atomic<bool> m_bReceiveFromServer = true;

	public:
		/**
		 * Starts the client. If an IP has been specified as a command-line argument, it is used. Otherwise prompts the user for input.
		 */
		void Start();
		/**
		 * Reaches out to the server and tries to connect.
		 */
		[[nodiscard]] oc::ReturnCode ConnectToServer(const sf::IpAddress& kIPAddress);

		// In reality, the server authenticates the client.
		[[nodiscard]] oc::ReturnCode AuthenticateToServer();

		[[nodiscard]] oc::ReturnCode SendPacket(oc::Packet& inPacket);

		[[nodiscard]] oc::ReturnCode ReceivePacket(oc::Packet& outPacket);

		void Disconnect();
	};
}
