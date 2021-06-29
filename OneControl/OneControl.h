#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <string_view>
#include <SFML/Network.hpp>
//#include <botan/botan.h>
//#include <cryptopp/rsa.h>
//#include <openssl/rsa.h>

#include "Constants.h"
#include "Enums.h"

namespace oc
{
	int32_t GetUserInt(const std::wstring_view& msg, const int32_t min, const int32_t max);
	sf::IpAddress GetUserIP(const std::wstring_view& msg);
	void ClearConsole();

	class OneControl
	{
	private:
		std::unique_ptr<sf::TcpSocket> m_pClient;
		eMachineState m_eState;

		void m_StartServer();
		void m_StartClient();
		void m_StartService();
		bool m_ReceiveAuthenticationPacket();
		bool m_SendAuthenticationPacket(std::unique_ptr<sf::TcpSocket>& socket);

	public:
		eMachineState GetMachineState();
		std::unique_ptr<sf::TcpSocket>& GetClient();
		void Start();
		void SetClient(std::unique_ptr<sf::TcpSocket>& client);
		void CreateClient();
		void CreateListener();
	};
}