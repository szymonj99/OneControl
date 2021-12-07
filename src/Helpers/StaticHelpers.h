#pragma once

#define UNICODE 1

#include <iostream>
#include <cstdint>
#include <string>

#include <SFML/Network.hpp>
#include <fmt/core.h>
#include <fmt/color.h>

namespace oc
{
	static void ClearConsole()
	{
		fmt::print("\033c"); // Alternative: "\033[2J"
	}

	static int32_t GetUserInt(const std::string& msg, const int32_t min, const int32_t max)
	{
		int32_t input = 0;
		fmt::print(msg);
		do
		{
			std::string inputString;
			std::getline(std::cin, inputString);
			try
			{
				input = std::stoi(inputString);
			}
			catch (std::invalid_argument)
			{
				fmt::print(fmt::fg(fmt::color::red), "Invalid input. Try again and stop trying to break this.\n");
				input = -1;
			}
			catch (std::out_of_range)
			{
				fmt::print(fmt::fg(fmt::color::red), "Invalid number chosen. Try again and stop trying to break this.\n");
				input = -1;
			}
			if (input > max || input == 0)
			{
				fmt::print(fmt::fg(fmt::color::red), "Invalid number chosen. Try again.\n");
				input = -1;
			}

			std::cin.clear();
		} while (input < min || input > max);
		return input;
	}

	static sf::IpAddress GetUserIP(const std::string& msg)
	{
		auto input = sf::IpAddress();
		do
		{
			fmt::print(msg);
			std::string inputString = std::string();
			std::getline(std::cin, inputString);
			if (!inputString.empty())
			{
				input = sf::IpAddress(inputString);
			}			
			std::cin.clear();
		}
		while (input == input.None);
		ClearConsole();
		fmt::print("Connecting to server IP: {}\n", input.toString());
		return input;
	}

	static oc::eMachineState GetMachineState()
	{
		const auto userInt = GetUserInt("Is this machine a Server or a Client?\n1. Server\n2. Client\n", 1, 2);
		return static_cast<eMachineState>(userInt - 1);
	}
}
