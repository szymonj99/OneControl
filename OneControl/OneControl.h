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
	class ocVersion
	{
	private:
		uint32_t m_Major = 0;
		uint32_t m_Minor = 0;
		uint32_t m_Revision = 1;

	public:
		ocVersion() {};
		ocVersion(const uint32_t major, const uint32_t minor, const uint32_t revision);
		std::wstring GetVersionStringView() const;
		uint32_t GetMajor() const;
		uint32_t GetMinor() const;
		uint32_t GetRevision() const;
	};

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