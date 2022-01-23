#pragma once

#define UNICODE 1

#include <iostream>
#include <cstdint>
#include <string>

#include <SFML/Network.hpp>
#include <fmt/core.h>
#include <fmt/color.h>

#include "Enums.h"

namespace oc
{
	static void ClearConsole()
	{
		fmt::print("\033c"); // Alternative: "\033[2J"
	}

	static int32_t GetUserInt(const std::string& msg, const int32_t min, const int32_t max)
	{
		if (max < min)
		{
			fmt::print(fmt::fg(fmt::color::red), "Incorrect min and max values received.\n");
			std::cin.get();
			exit(-1);
		}

		fmt::print(msg);
		int32_t input = 0;
		char* end = nullptr;
		bool failedToParse = true;		
		do
		{
			std::string inputString;
			std::getline(std::cin, inputString);

			errno = 0;
            input = static_cast<int32_t>(strtol(inputString.c_str(), &end, 0));
			// *end == 0 if we correctly parsed the whole input.
			// if *end is not null, there were characters that were not parsed.
			failedToParse = (*end != '\0');

			if (failedToParse)
			{
				fmt::print(fmt::fg(fmt::color::red), "The provided input is not a number.\n");
			}
			else
			{
				if (input > max)
				{
					fmt::print(fmt::fg(fmt::color::red), "The provided number is too big.\n");
				}
				else if (input < min)
				{
					fmt::print(fmt::fg(fmt::color::red), "The provided number is too small.\n");
				}
			}

			std::cin.clear();
		} while ((input < min || input > max) || failedToParse);
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
		return static_cast<eMachineState>(userInt);
	}
}
