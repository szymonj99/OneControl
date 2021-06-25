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
//#include <botan/botan.h>
//#include <cryptopp/rsa.h>
//#include <openssl/rsa.h>

#include "Constants.h"

namespace oc
{
	void ClearConsole()
	{
		std::wcout << L"\033c";
	}

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
		uint32_t m_Major = 0;
		uint32_t m_Minor = 0;
		uint32_t m_Revision = 1;

	public:
		ocVersion() {};
		ocVersion(const uint32_t major, const uint32_t minor, const uint32_t revision)
		{
			m_Major = major;
			m_Minor = minor;
			m_Revision = revision;
		}

		std::wstring GetVersionStringView() const
		{
			return L"" + std::to_wstring(m_Major) + L"." + std::to_wstring(m_Minor) + L"." + std::to_wstring(m_Revision);
		}

		uint32_t GetMajor() const
		{
			return m_Major;
		}
		uint32_t GetMinor() const
		{
			return m_Minor;
		}
		uint32_t GetRevision() const
		{
			return m_Revision;
		}
	};

	static ocVersion Version(0,0,1);
	
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
			catch (std::invalid_argument)
			{
				std::wcout << L"Invalid input. Try again and stop trying to break this.\n";
				input = -1;
			}
			catch (std::out_of_range)
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

	sf::IpAddress GetUserIP(const std::wstring_view& msg)
	{
		auto input = sf::IpAddress::IpAddress();
		std::wcout << msg;
		do
		{
			std::wstring inputString = std::wstring();
			std::getline(std::wcin, inputString);
			input = sf::IpAddress::IpAddress((char*)(inputString.c_str()));
			std::wcin.clear();
		}
		// Limiting to LAN for now (not great at all).
		// Won't work over VPN eg. WireGuard.
		while (input < sf::IpAddress(192, 168, 1, 1) || input > sf::IpAddress(192, 168, 1, 254));

		return input;
	}

	class OneControl
	{		
	private:
		//using vClients = std::vector<sf::TcpSocket*>;
		//vClients m_vpClients;
		std::unique_ptr<sf::TcpSocket> m_pClient;
		eMachineState m_eState;

		void m_StartServer()
		{
			std::thread listenerThread([&] { CreateListener(); });
			listenerThread.join();
			std::wcout << L"Listener thread finished.\n";
		}

		void m_StartClient()
		{
			std::thread clientThread([&] { CreateClient(); });			
			clientThread.join();
			std::wcout << L"Client thread finished.\n";
		}

		void m_StartService()
		{
			if (m_eState == eMachineState::Server)
			{
				m_StartServer();
			}
			else if (m_eState == eMachineState::Client)
			{
				m_StartClient();
			}
		}

	public:
		eMachineState GetMachineState()
		{
			const auto userInt = GetUserInt(std::wstring_view(L"Is this machine a Server or a Client?\n1. Server\n2. Client\n"), 1, 2);
			return static_cast<eMachineState>(userInt - 1);
		}

		std::unique_ptr<sf::TcpSocket>& GetClient()
		{
			return m_pClient;
		}

		void Start()
		{
			m_eState = GetMachineState();
			ClearConsole();
			m_StartService();
		}

		void SetClient(std::unique_ptr<sf::TcpSocket>& client)
		{
			m_pClient = std::move(client);
		}

		void CreateClient()
		{
			auto socket = std::make_unique<sf::TcpSocket>();
			auto serverIP = GetUserIP(std::wstring_view(L"Insert server IP\n"));
			auto status = socket->connect(serverIP.toString(), oc::port);

			if (status != sf::Socket::Status::Done)
			{
				std::wcout << L"Client can't connect to server.\n";
			}

			// Else we are connected to the server!
			std::wcout << L"Connected to server successfully.\n";

			// Authentication packet
			{

				auto authenticationPkt = sf::Packet();
				
				authenticationPkt << Version.GetMajor() << Version.GetMinor() << Version.GetRevision();
				if (socket->send(authenticationPkt) != sf::Socket::Status::Done)
				{
					socket->disconnect();
				}
				std::wcout << L"Client authentication successful.\n";
			}

			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
				sf::Packet pkt;
				pkt << std::wstring(L"Hello");
				if (socket->send(pkt) != sf::Socket::Status::Done)
				{
					socket->disconnect();
					std::wcout << L"Client lost connection with server.\nQuitting.\n";
					return;
				}
				std::wstring str;
				pkt >> str;
				std::wcout << L"I'm a client. Sending packet to server. Packet: " << str << L"\n";
			}
		}

		void CreateListener()
		{
			auto listener = std::make_unique<sf::TcpListener>();

			if (listener->listen(oc::port) != sf::Socket::Status::Done)
			{
				std::wcout << L"Can't create TCP listener on port " << oc::port << L"\n";
				return;
			}

			auto client = std::make_unique<sf::TcpSocket>();

			std::wcout << L"Waiting for client.\n";

			if (listener->accept(*client) != sf::Socket::Status::Done)
			{
				std::wcout << L"Can't create client on port " << oc::port << L"\n";
				return;
			}

			// c_str() looks ugly but can't convert from string to wstring.
			std::wcout << L"We have a client! IP: " << client->getRemoteAddress().toString().c_str() << L"\n";
			SetClient(client);
			std::wcout << L"We receive client messages here.\n";

			// Authenticate client with program version
			// If initial packet containing the version doesn't match server version, kick the client.
			// Else, receive packets in while loop.

			{
				auto authenticationPkt = sf::Packet();
				if (GetClient()->receive(authenticationPkt) != sf::Socket::Status::Done)
				{
					std::wcout << L"Failed at getting authentication packet.\nQuitting.\n";
					return;
				}
				std::uint32_t major, minor, revision;				
				authenticationPkt >> major >> minor >> revision;

				ocVersion version(major, minor, revision);
				if (version.GetVersionStringView() != Version.GetVersionStringView())
				{
					std::wcout << L"Version mismatch!!!\nClient version: " + std::wstring(version.GetVersionStringView()) + L"\n";
					std::wcout << L"Server version: " + std::wstring(Version.GetVersionStringView()) + L"\nKicking client.\n";
					GetClient()->disconnect();
				}
				std::wcout << "Client authentication successful.\n";
			}			

			while (true)
			{
				sf::Packet pkt = sf::Packet();
				if (GetClient()->receive(pkt) != sf::Socket::Status::Done)
				{
					std::wcout << L"Client disconnected.\nGracefully quitting.\n";
					return;
				}
				std::wstring data;
				pkt >> data;
				std::wcout << data << L"\n";
			}					
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
	std::wcin.get();
	return 0;	
}
