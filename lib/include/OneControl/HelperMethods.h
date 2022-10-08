#pragma once

#include <iostream>
#include <cstdint>
#include <string>

#include <SFML/Network/IpAddress.hpp>
#include <fmt/core.h>
#include <fmt/color.h>
#include <argh.h>

#include <OneControl/Constants.h>
#include <OneControl/Enums.h>
#include <OneControl/RuntimeGlobals.h>

namespace oc
{
	static void ClearConsole()
	{
		//std::cout << "\033c" << std::endl;
		fmt::print("\033c"); // Alternative: "\033[2J"
	}

	template<class T>
	static T GetUserValue(const std::string& msg, const T min, const T max)
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
		const auto userValue = GetUserValue("Is this machine a Server or a Client?\n1. Server\n2. Client\n", (int32_t)1, (int32_t)2);
		return static_cast<oc::eMachineState>(userValue);
	}

	static void PrintHelp()
	{
		fmt::print("Welcome to OneControl.\n");
		fmt::print("\n");
		fmt::print("OPTIONS:\n");
		fmt::print("\n");
		fmt::print("\t-h, --help                   : Show the help banner.\n");
		fmt::print("\t-p, --port <port>            : The port on which to connect to/listen on. If no port is set, it will use the default port of {}\n", oc::kPort);
		fmt::print("\t-t, --type <client/server>   : Set this machine to act as either the server or the client.\n");
		fmt::print("\t-s, --server <ip>            : The server address to which to connect to. Only applicable if this machine will act as a client.\n");
		fmt::print("\n");
		fmt::print("Example usage:\n");
		fmt::print("\n");
		fmt::print("\tOneControl -p 12345                            : Change the port number to 12345\n");
		fmt::print("\tOneControl -t server                           : This machine will act as the server\n");
		fmt::print("\tOneControl -p 12345 -t server                  : This machine will act as the server on port 12345\n");
		fmt::print("\tOneControl -p 12345 -t client -s 192.168.1.111 : This machine will act as the client and will try to connect to the server with an IP of 192.168.1.111 on port 12345\n");
		fmt::print("\tOneControl --no-mouse						     : This machine will not send mouse events to the client, or receive mouse events from the server\n");
		fmt::print("\tOneControl --no-keyboard					     : This machine will not send keyboard events to the client, or receive keyboard events from the server\n");
	}

	// Let's refactor this to make the code a bit nicer; it works but it ain't as pretty as it could be.
	static void ParseArgs(const int argc, char* argv[])
	{
		if (argc == 1)
		{
			return;
		}

		argh::parser cmdl;
		// 'Registering' options by calling `add_params` means that the param will have a value that is not a boolean.
		// Not registering a param will default to a bool.
		cmdl.add_params({ "-p", "--port", "--s", "--server", "-t", "--type" });
		cmdl.parse(argv);

		if (cmdl[{"-h", "--help"}])
		{
			oc::PrintHelp();
			std::exit(0);

		}

		std::string output = "Running OneControl with the following options:\n";
		auto printOutput = false;

		if (cmdl[{"--no-mouse"}])
		{
			printOutput = true;
			output.append("\tMouse disabled.\n");
			oc::RuntimeGlobals::mouseEnabled = false;
		}

		if (cmdl[{"--no-keyboard"}])
		{
			printOutput = true;
			output.append("\tKeyboard disabled.\n");
			oc::RuntimeGlobals::keyboardEnabled = false;
		}

		// Let's check if the user isn't doing something silly.
		if (!oc::RuntimeGlobals::mouseEnabled && !oc::RuntimeGlobals::keyboardEnabled)
		{
			fmt::print(stderr, "Can't run OneControl with mouse AND keyboard disabled. Remove either the `--no-keyboard` or `--no-mouse` parameter and launch the application again.");
			// Let's make oc::ReturnCodes::Failure or something here.
			std::exit(1);
		}

		if (!cmdl({ "-t", "--type" }).str().empty())
		{
			printOutput = true;
			std::string type;
			cmdl({ "-t", "--type" }) >> type;

			if (type == "client" || type == "c")
			{
				oc::RuntimeGlobals::isClient = true;
				oc::RuntimeGlobals::isServer = !oc::RuntimeGlobals::isClient;
				output.append("\tThis will be the client.\n");
			}

			if (type == "server" || type == "s")
			{
				oc::RuntimeGlobals::isClient = false;
				oc::RuntimeGlobals::isServer = !oc::RuntimeGlobals::isClient;
				output.append("\tThis will be the server.\n");
			}

			if (!oc::RuntimeGlobals::isServer && !oc::RuntimeGlobals::isClient)
			{
				fmt::print(stderr, "Invalid type provided: {}\n", type);
				std::exit(1);
			}
		}

		if (!cmdl({ "-p", "--port" }).str().empty())
		{
			if (!(cmdl({ "-p", "--port" }) >> oc::RuntimeGlobals::port) || oc::RuntimeGlobals::port == 0)
			{
				fmt::print(stderr, "Invalid port provided: {}\n", cmdl({"-p", "--port"}).str());
				// Let's make oc::ReturnCodes::Failure or something here.
				std::exit(1);
			}

			oc::RuntimeGlobals::customPort = true;
			printOutput = true;
			output.append("\tRunning on port: " + std::to_string(oc::RuntimeGlobals::port) + "\n");
		}

		if (!cmdl({ "-s", "--server" }).str().empty())
		{
			if (oc::RuntimeGlobals::isServer)
			{
				fmt::print(stderr, "Can't connect to a server when this machine will act as the server.\n");
				std::exit(1);
			}

			std::string serverStr;
			cmdl({ "-s", "--server" }) >> serverStr;
			
			oc::RuntimeGlobals::serverIP = sf::IpAddress(serverStr);

			if (oc::RuntimeGlobals::serverIP == sf::IpAddress::None)
			{
				fmt::print(stderr, "Invalid server IP address provided: {}\n", serverStr);
				std::exit(1);
			}

			oc::RuntimeGlobals::customServerIP = true;
			printOutput = true;
			output.append("\tConnecting to Server IP: " + oc::RuntimeGlobals::serverIP.toString() + "\n");
		}

		if (printOutput)
		{
			fmt::print(fmt::runtime(output));
		}
	}
}
