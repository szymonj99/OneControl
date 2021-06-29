#include "OneControl.h"
#include "Version.h"

// The version of the application.
static oc::ocVersion Version(0, 0, 1);

void oc::ClearConsole()
{
	std::wcout << L"\033c";
}

int32_t oc::GetUserInt(const std::wstring_view& msg, const int32_t min, const int32_t max)
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

sf::IpAddress oc::GetUserIP(const std::wstring_view& msg)
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



void oc::OneControl::m_StartClient()
{
	std::thread clientThread([&] { CreateClient(); });
	clientThread.join();
	std::wcout << L"Client thread finished.\n";
}

void oc::OneControl::m_StartService()
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

bool oc::OneControl::m_ReceiveAuthenticationPacket()
{
	auto authenticationPkt = sf::Packet();
	if (GetClient()->receive(authenticationPkt) != sf::Socket::Status::Done)
	{
		std::wcout << L"Failed at getting authentication packet.\nQuitting.\n";
		return false;
	}
	std::uint32_t major, minor, revision;
	authenticationPkt >> major >> minor >> revision;

	ocVersion version(major, minor, revision);
	if (version.GetVersionStringView() != Version.GetVersionStringView())
	{
		std::wcout << L"Version mismatch!!!\nClient version: " + std::wstring(version.GetVersionStringView()) + L"\n";
		std::wcout << L"Server version: " + std::wstring(Version.GetVersionStringView()) + L"\nKicking client.\n";
		GetClient()->disconnect();
		return false;
	}
	std::wcout << "Client authentication successful.\n";
	return true;
}

bool oc::OneControl::m_SendAuthenticationPacket(std::unique_ptr<sf::TcpSocket>& socket)
{
	auto authenticationPkt = sf::Packet();

	authenticationPkt << Version.GetMajor() << Version.GetMinor() << Version.GetRevision();
	if (socket->send(authenticationPkt) != sf::Socket::Status::Done)
	{
		std::wcout << L"Client authentication FAILED.\n";
		return false;
	}
	std::wcout << L"Client authentication successful.\n";
	return true;
}

oc::eMachineState oc::OneControl::GetMachineState()
{
	const auto userInt = GetUserInt(std::wstring_view(L"Is this machine a Server or a Client?\n1. Server\n2. Client\n"), 1, 2);
	return static_cast<eMachineState>(userInt - 1);
}

std::unique_ptr<sf::TcpSocket>& oc::OneControl::GetClient()
{
	return m_pClient;
}

void oc::OneControl::Start()
{
	m_eState = GetMachineState();
	ClearConsole();
	m_StartService();
}



void oc::OneControl::CreateClient()
{
	auto socket = std::make_unique<sf::TcpSocket>();
	auto serverIP = GetUserIP(std::wstring_view(L"Insert server IP\n"));
	auto status = socket->connect(serverIP.toString(), oc::port);

	if (status != sf::Socket::Status::Done)
	{
		std::wcout << L"Client can't connect to server.\n";
		std::wcin.get();
		return;
	}

	std::wcout << L"Connected to server successfully.\n";

	if (!m_SendAuthenticationPacket(socket))
	{
		socket->disconnect();
		std::wcin.get();
		return;
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
			std::wcin.get();
			return;
		}
		std::wstring str;
		pkt >> str;
		std::wcout << L"I'm a client. Sending packet to server. Packet: " << str << L"\n";
	}
}

