#pragma once

#define UNICODE 1
#ifdef _WIN32
// Windows 10
#define _WIN32_WINNT 0x0A00
#define WINDOWS 1
#elif __APPLE__
#define APPLE 1
#elif __linux__
#define LINUX 1
#else
#error Platform is not known/supported.
#endif

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <string_view>
#include <SFML/Network.hpp>
#include <botan/botan.h>
#include <cryptopp/rsa.h>
#include <openssl/rsa.h>

#include "Constants.h"

namespace oc
{
	enum class eMachineState
	{
		Server,
		Client
	};

	enum class eEvents
	{
		MouseMoveDown,
		MouseMoveUp,
		KeyPressDown,
		KeyPressUp,
		ScrollMoveUp,
		ScrollMoveDown,
		ScrollPressUp,
		ScrollPressDown
	};

	class cMessageHeader
	{
	private:
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		TimePoint m_TimePoint = Clock::now();
		uint64_t m_ID;

	public:
		cMessageHeader()
		{
			m_ID = m_TimePoint.time_since_epoch().count();
		}
		cMessageHeader(uint64_t id)
		{
			m_ID = id;
		}

		uint64_t getID() const
		{
			return m_ID;
		}
	};

	class cMessageBody
	{
	public:
		eEvents m_Event;
		wchar_t m_Button;
	};

	// This will be sent over TCP
	class cMessage
	{
	private:
		cMessageHeader m_Header;
		cMessageBody m_Body;

	public:
		friend std::wostream& operator << (std::wostream& os, const cMessage& msg)
		{
			os << L"ID: " << msg.m_Header.getID();
			return os;
		}
	};

	class ocVersion
	{
	private:
		uint8_t m_Major = 0;
		uint8_t m_Minor = 0;
		uint8_t m_Revision = 1;
	};

	void CreateListener()
	{
		sf::TcpListener listener;

		if (listener.listen(oc::port) != sf::Socket::Status::Done)
		{
			// Error here.
		}

		sf::TcpSocket client;

		if (listener.accept(client) != sf::Socket::Done)
		{
			// Error here.
		}

		// Else check the client validity (client version)
		// And boot them off if versions do not match.

		// Else add client 
	}

	int32_t GetUserInt(const std::wstring_view& msg, const int32_t min, const int32_t max)
	{
		int32_t input = 0;
		std::wcout << msg;
		do
		{
			std::wstring inputString = std::wstring();
			std::getline(std::wcin, inputString);
			try
			{
				input = std::stoi(inputString);
			}
			catch (const std::invalid_argument& e)
			{
				std::wcout << L"Invalid input. Try again and stop trying to break this.\n";
				input = -1;
			}
			catch (const std::out_of_range& e)
			{
				std::wcout << L"Invalid number chosen. Try again and stop trying to break this.\n";
				input = -1;
			}
			if (input > max || input == 0)
			{
				std::wcout << L"Invalid number chosen. Try again.\n";
				input = -1;
			}

			std::wcin.clear();
		} while (input < min || input > max);
		return input;
	}

	class OneControl
	{		
	private:
		using vClients = std::vector<sf::TcpSocket*>;
		eMachineState m_State;
		vClients m_vpClients;
		sf::TcpSocket* m_pClient;
		eMachineState m_eState;

	public:
		eMachineState GetMachineState()
		{
			const auto userInt = GetUserInt(const std::wstring_view(L"Is this machine a Server or a Client?\n1. Server\n2. Client\n"), 1, 2);
			return static_cast<eMachineState>(userInt);
		}

		void Start()
		{
			m_eState = GetMachineState();
		}

		void Stop()
		{
			// Clean-up here.
		}
	};
}

int main()
{	
	{
		std::wcout << L"OneControl is starting.\n";
		auto oneControl = std::make_unique<oc::OneControl>();
		oneControl->Start();
	}
	std::wcout << L"Program finished. Press enter to exit.";
	std::cin.get();
	return 0;	
}
