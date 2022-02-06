#pragma once

#define UNICODE 1

#include <iostream>
#include <cstdint>
#include <string>

#include <SFML/Network/IpAddress.hpp>
#include <fmt/core.h>
#include <fmt/color.h>
#include <argh.h>

#include "Enums.h"
#include "Constants.h"

namespace oc
{
	static void ClearConsole()
	{
		fmt::print("\033c"); // Alternative: "\033[2J"
	}

	template<class T>
	static T GetUserInt(const std::string& msg, const T min, const T max)
	{
		if (max < min)
		{
			fmt::print(fmt::fg(fmt::color::red), "Incorrect min and max values received.\n");
			std::cin.get();
			exit(-1);
		}

		fmt::print(fmt::runtime(msg));
		T input = 0;
		char* end = nullptr;
		bool failedToParse = true;		
		do
		{
			std::string inputString;
			std::getline(std::cin, inputString);

			errno = 0;
            input = static_cast<T>(strtol(inputString.c_str(), &end, 0));
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
			fmt::print(fmt::runtime(msg));
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
		const auto userInt = GetUserInt("Is this machine a Server or a Client?\n1. Server\n2. Client\n", (int32_t)1, (int32_t)2);
		return static_cast<oc::eMachineState>(userInt);
	}

	// This whole thing really needs to be reworked.
	static void ParseFlags(const int argc, char* argv[])
	{
		// if argc == 1, then we didn't pass in any arguments to parse.
		if (argc > 1)
		{
			argh::parser parser;
			parser.add_params({ "-p", "--port", "--s", "--server", "-t", "--type" });
			parser.parse(argc, argv);

			if (parser["-h"] || parser["--help"])
			{
				fmt::print("Welcome to OneControl.\n");
				fmt::print("\n");
				fmt::print("OPTIONS:\n");
				fmt::print("\n");
				fmt::print("    -h, --help                   Show the help banner.\n");
				fmt::print("    -p, --port <port>            The port on which to connect to/listen on. If no port is set, it will use the default port of {}\n", oc::kPort);
				fmt::print("    -t, --type <client/server>   Set this machine to act as either the server or the client.\n");
				fmt::print("    -s, --server <ip>            The server address to which to connect to. Only applicable if this machine will act as a client.\n");
				fmt::print("\n");
				fmt::print("Example usage:\n");
				fmt::print("\n");
				fmt::print("    OneControl -p 12345                            : Change the port number to 12345\n");
				fmt::print("    OneControl -t server                           : This machine will act as the server\n");
				fmt::print("    OneControl -p 12345 -t server                  : This machine will act as the server on port 12345\n");
				fmt::print("    OneControl -p 12345 -t client -s 192.168.1.111 : This machine will act as the client and will try to connect to the server with an IP of 192.168.1.111 on port 12345\n");

				std::exit(0);
			}

			bool printOutput = false;
			std::string output = "\nRunning OneControl with the following options:\n";

			if (!parser("-p").str().empty() || !parser("--port").str().empty())
			{
				printOutput = true;
				uint16_t port = 0;
				char* end = nullptr;
				bool failedToParse = true;
				std::string inputString = !parser("-p").str().empty() ? parser("-p").str() : parser("--port").str();

				errno = 0;
				port = static_cast<uint16_t>(strtol(inputString.c_str(), &end, 0));
				// *end == '\0' if we correctly parsed the whole input.
				// if *end is not empty, there were characters that were not parsed.
				failedToParse = (*end != '\0');

				if (failedToParse)
				{
					fmt::print(fmt::fg(fmt::color::red), "Invalid argument passed into the port parameter.\n");
					std::exit(0);
				}

				output += fmt::format("Port: {}\n", port);
			}

			std::string typeInput;
			if (!parser("-t").str().empty() || !parser("--type").str().empty())
			{
				printOutput = true;
				const std::vector<std::string> kAllowedValues = { "s", "server", "c", "client" };
				typeInput = !parser("-t").str().empty() ? parser("-t").str() : parser("--type").str();

				// Rework this
				bool validString = false;
				for (const std::string& str : kAllowedValues)
				{
					if (typeInput == str)
					{
						validString = true;
						output += fmt::format("Type: {}\n", typeInput == "c" || typeInput == "client" ? "Client" : "Server");
						break;
					}
				}
				if (!validString)
				{
					fmt::print(fmt::fg(fmt::color::red), "Invalid type string {}\nOnly [c, client, s, server] values are allowed.\n", typeInput);
					std::exit(0);
				}
			}

			if (!parser("-s").str().empty() || !parser("--server").str().empty())
			{
				if (typeInput == "client" || typeInput == "c")
				{
					printOutput = true;
					sf::IpAddress ipAddress;
					try
					{
						ipAddress = !parser("-s").str().empty() ? sf::IpAddress(parser("-s").str()) : sf::IpAddress(parser("--server").str());
					}
					catch (const std::exception e)
					{
						fmt::print(fmt::fg(fmt::color::red), "{}\n", e.what());
						std::exit(0);
					}

					if (ipAddress.toInteger() == 0)
					{
						fmt::print(fmt::fg(fmt::color::red), "Failed when trying to parse server IP: {}\n", !parser("-s").str().empty() ? parser("-s").str() : parser("--server").str());
						std::exit(0);
					}

					output += fmt::format("Server: {}\n", ipAddress.toString());
				}
				else
				{
					fmt::print(fmt::fg(fmt::color::red), "The server IP can only be set when this machine acts as a client. Consider using the '--type client' flag.\n");
					std::exit(0);
				}
			}

			if (printOutput)
			{
				fmt::print("{}\n", output);
			}
		}
		else
		{
			fmt::print("OneControl is starting.\n");
		}
	}
}
