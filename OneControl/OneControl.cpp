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

#include "Constants.h"
#include "RSA.h"

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
			os << std::wstring_view(L"ID: ") << msg.m_Header.getID();
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

		if (listener.listen(oc::port) != sf::Socket::Done)
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

	class OneControl
	{
		using vClients = std::vector<sf::TcpSocket*>;
	private:
		eMachineState m_State;
		vClients m_vpClients;
		sf::TcpSocket* m_pClient;
		eMachineState m_eState;



	public:

		//eMachineState GetMachineState()
		//{
		//	uint32_t input;

		//	while (true)
		//	{
		//		std::wcout << std::wstring_view(L"Is this machine a Server or a Client?\n1. Server\n2. Client\n");
		//		std::wstring temp;				
		//		std::wcin >> temp;

		//		//strtoi

		//		bool isInt = false;
		//		for (auto c : temp)
		//		{
		//			if (c >= 0 && c <= 9)
		//			{
		//				//std::cout << c;
		//				isInt = true;
		//			}
		//			else
		//			{
		//				isInt = false;
		//				break;
		//			}
		//		}

		//		if (isInt)
		//		{
		//			input = std::stoi(temp);
		//			break;
		//		}				
		//	}

		//	return eMachineState::Client;
		//}

		void SetSeed()
		{
			constexpr uint32_t loops = 50000;
			srand(0);
			for (uint32_t loop = 0; loop < loops; loop++)
			{
				srand(static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count() * rand()));
			}

			for (int i = 0; i < 10; i++)
			{
				std::cout << GetRandom() << " " << GetRandom() << "\n";
			}
			
			for (int i = 0; i < 10; i++)
			{
				auto x = GetRandom();
				std::cout << GetCubanPrime(x / 1000, (x / 1000)- 2) << "\n";
			}
			
		}

		void Start()
		{
			// Set the seed for random numbers
			SetSeed();

			////GetMachineState();
			//auto prime = oc::GetCubanPrime(6000, 5898);
			//prime *= prime;
			//prime *= prime;
			//prime *= prime;
			////std::cout << std::to_string();
			//std::cout << prime << "\n";
			//std::cout << "115792089237316195423570985008687907853269984665640564039457584007913129639935\n";
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
		std::wcout << std::wstring_view(L"OneControl is starting.\n");
		auto oneControl = std::make_unique<oc::OneControl>();
		oneControl->Start();
	}
	std::wcout << std::wstring_view(L"Program finished. Press enter to exit.");
	std::cin.get();
	return 0;	
}
