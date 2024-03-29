#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <csignal>
#include <exception>
#include <stdexcept>

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
#include <OneLibrary/ThreadsafeQueue.h>
#include <OneLibrary/Input.h>
#include <OneControl/Packet.h>
#include <OneControl/Constants.h>
#include <OneControl/Version.h>
#include <OneControl/RuntimeGlobals.h>
#include <OneControl/ReturnCode.h>
#include <OneControl/Crypto.h>

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
		oc::ReturnCode m_fReceiveAuthenticationPacket();
        oc::ReturnCode m_fHandshake();
        ol::ThreadsafeQueue<ol::Input> m_bufInputs{};
		std::atomic<bool> m_bSendToClient = true;

	public:
		~Server() noexcept { this->m_bufInputs.Interrupt(); }

        /**
         * Start the server.
         */
		void Start();
        /**
         * Wait until a client has attempted a connection to this server.
         */
		[[nodiscard]] oc::ReturnCode WaitForClient();

		/**
		 * Authenticate the client using a mutual handshake.
		 */
		[[nodiscard]] oc::ReturnCode AuthenticateClient();

		[[nodiscard]] ol::Input GetNextInput() noexcept(false);

		[[nodiscard]] oc::ReturnCode SendPacket(oc::Packet& inPacket);

        [[nodiscard]] oc::ReturnCode ReceivePacket(oc::Packet& outPacket);

		void Disconnect();
		// TODO: Add Shutdown function here.
	};
}
