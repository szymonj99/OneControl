#pragma once

#ifdef OS_WINDOWS
    #include <Windows.h>
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
#include <OneControl/RuntimeGlobals.h>
#include <OneControl/Input.h>
#include <OneLibrary/InputGathererMouse.h>
#include <OneLibrary/InputGathererKeyboard.h>

namespace oc
{
    /**
     * This class is used when the current selected machine acts as the server, and shares its inputs to the client.
     */
	class Server : sf::TcpSocket
	{
	private:
		std::unique_ptr<sf::TcpListener> m_pListener{};
        // TODO: Check if the client can be of type oc::Client, rather than a raw SFML TCP socket.
		std::unique_ptr<sf::TcpSocket> m_pClient{};
		bool m_ReceiveAuthenticationPacket();

	public:
        /**
         * Start the server.
         */
		void Start();
        /**
         * Wait until a client has attempted a connection to this server.
         */
		void WaitForClient();
        /**
         * Start the server loop where the mouse and/or keyboard input will be shared with the client.
         */
		void ServerLoop();
        /**
         * Send a packet to the currently connected client.
         * @return True if the packet was sent. False otherwise.
         */
		bool SendPacketToClient(oc::Packet& kPacket);
	};
}
