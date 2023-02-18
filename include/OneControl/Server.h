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
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/base64.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

#include <OneLibrary/InputGathererMouse.h>
#include <OneLibrary/InputGathererKeyboard.h>
#include <OneControl/Packet.h>
#include <OneControl/Constants.h>
#include <OneControl/Version.h>
#include <OneControl/RuntimeGlobals.h>
#include <OneControl/Input.h>
#include <OneControl/ReturnCode.h>

namespace oc
{
    /**
     * This class is used when the current selected machine acts as the server, and shares its inputs to the client.
     */
	class Server : sf::TcpSocket
	{
	private:
        std::unique_ptr<sf::TcpListener> m_pListener = std::make_unique<sf::TcpListener>();
        // TODO: Check if the client can be of type oc::Client, rather than a raw SFML TCP socket.
        std::unique_ptr<sf::TcpSocket> m_pClient = std::make_unique<sf::TcpSocket>();
		oc::ReturnCode m_ReceiveAuthenticationPacket();
        oc::ReturnCode m_Handshake();

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
	};
}
