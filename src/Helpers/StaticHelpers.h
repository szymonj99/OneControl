#pragma once

#include <iostream>
#include <cstdint>
#include <string>

#include <SFML/Network.hpp>

namespace oc
{
	static void ClearConsole()
	{
		std::cout << "\033c";
		std::cout << "\033[2J";
		//printf("\033[2J");
	}

	static int32_t GetUserInt(const std::string& msg, const int32_t min, const int32_t max)
	{
		int32_t input = 0;
		std::cout << msg;
		do
		{
			std::string inputString = std::string();
			std::getline(std::cin, inputString);
			try
			{
				input = std::stoi(inputString);
			}
			catch (std::invalid_argument)
			{
				std::cout << "Invalid input. Try again and stop trying to break this.\n";
				input = -1;
			}
			catch (std::out_of_range)
			{
				std::cout << "Invalid number chosen. Try again and stop trying to break this.\n";
				input = -1;
			}
			if (input > max || input == 0)
			{
				std::cout << "Invalid number chosen. Try again.\n";
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
			std::cout << msg;
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
		std::cout << "Connecting to server IP: " << input.toString() << "\n";
		return input;
	}

	static oc::eMachineState GetMachineState()
	{
		const auto userInt = GetUserInt("Is this machine a Server or a Client?\n1. Server\n2. Client\n", 1, 2);
		return static_cast<eMachineState>(userInt - 1);
	}
}
