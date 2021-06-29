#pragma once

#include <iostream>
#include <cstdint>
#include <string>

#include <SFML/Network.hpp>

namespace oc
{
	static void ClearConsole()
	{
		std::wcout << L"\033c";
	}

	static int32_t GetUserInt(const std::wstring_view& msg, const int32_t min, const int32_t max)
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

	static sf::IpAddress GetUserIP(const std::wstring_view& msg)
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

	static oc::eMachineState GetMachineState()
	{
		const auto userInt = GetUserInt(std::wstring_view(L"Is this machine a Server or a Client?\n1. Server\n2. Client\n"), 1, 2);
		return static_cast<eMachineState>(userInt - 1);
	}
}
