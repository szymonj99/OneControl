#include "OneControl.h"

// The version of the application.
static oc::ocVersion Version(0, 0, 1);

oc::ocVersion::ocVersion(const uint32_t major, const uint32_t minor, const uint32_t revision)
{
	m_Major = major;
	m_Minor = minor;
	m_Revision = revision;
}

std::wstring oc::ocVersion::GetVersionStringView() const
{
	return L"" + std::to_wstring(m_Major) + L"." + std::to_wstring(m_Minor) + L"." + std::to_wstring(m_Revision);
}

uint32_t oc::ocVersion::GetMajor() const
{
	return m_Major;
}
uint32_t oc::ocVersion::GetMinor() const
{
	return m_Minor;
}
uint32_t oc::ocVersion::GetRevision() const
{
	return m_Revision;
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

void oc::ClearConsole()
{
	std::wcout << L"\033c";
}

void oc::OneControl::m_StartServer()
{
	std::thread listenerThread([&] { CreateListener(); });
	listenerThread.join();
	std::wcout << L"Listener thread finished.\n";
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
		socket->disconnect();
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

void oc::OneControl::SetClient(std::unique_ptr<sf::TcpSocket>& client)
{
	m_pClient = std::move(client);
}

void oc::OneControl::CreateClient()
{
	auto socket = std::make_unique<sf::TcpSocket>();
	auto serverIP = GetUserIP(std::wstring_view(L"Insert server IP\n"));
	auto status = socket->connect(serverIP.toString(), oc::port);

	if (status != sf::Socket::Status::Done)
	{
		std::wcout << L"Client can't connect to server.\n";
	}

	std::wcout << L"Connected to server successfully.\n";

	if (!m_SendAuthenticationPacket(socket))
	{
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
			return;
		}
		std::wstring str;
		pkt >> str;
		std::wcout << L"I'm a client. Sending packet to server. Packet: " << str << L"\n";
	}
}

void oc::OneControl::CreateListener()
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

	if (!m_ReceiveAuthenticationPacket())
	{
		return;
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