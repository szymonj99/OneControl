#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <algorithm>
#include <cassert>

#include <SFML/Network/IpAddress.hpp>
#include <fmt/core.h>
#include <fmt/color.h>
#include <args.hxx>

#include <OneControl/Constants.h>
#include <OneControl/Enums.h>
#include <OneControl/RuntimeGlobals.h>
#include <OneControl/ReturnCode.h>

namespace oc
{
    /**
     * Clears the console using a special character code.
     * May not be supported in all terminals, such as Windows CMD.
     */
	static void ClearConsole()
	{
		//std::cout << "\033c" << std::endl;
		fmt::print("\033c"); // Alternative: "\033[2J"
	}

	static uint64_t GetUserValue(const std::string& msg, const uint64_t min, const uint64_t max)
	{
        assert(max > min);
        fmt::print(fmt::runtime(msg));
        uint64_t input = 0;
		char* end = nullptr;
		bool failedToParse = true;		
		do
		{
			std::string inputString;
			std::getline(std::cin, inputString);
			errno = 0;
            input = strtoull(inputString.c_str(), &end, 0);
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

    /**
     * Get an IP address from the user. It will prompt the user to keep inputting an IP address until a valid IP address is input.
     * @param msg The message that will be displayed before asking the user for input
     * @return An IP address
     */
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
		while (input == sf::IpAddress::None);
		ClearConsole();
		fmt::print("Connecting to server IP: {}\n", input.toString());
		return input;
	}

    /**
     * Get the current machine state, stating if it's a server or a client.
     * @return The machine state
     */
	static oc::eMachineState GetMachineState()
	{
        if (oc::RuntimeGlobals::isClient) { return oc::eMachineState::Client; }
        if (oc::RuntimeGlobals::isServer) { return oc::eMachineState::Server; }
		const auto userValue = GetUserValue("Is this machine a Server or a Client?\n1. Server\n2. Client\n", (int64_t)1, (int64_t)2);
		return static_cast<oc::eMachineState>(userValue);
	}

    /**
     * Parse command line arguments/flags, and change runtime options.
     * @param argc The number of parameters passed in
     * @param argv An array of parameters/flags passed in
     * @return
     */
    static oc::ReturnCode ParseArguments(const int argc, const char* argv[])
    {
        if (argc < 2) { return oc::ReturnCode::Success; }

        // Design:
        // Would we prefer having to call:
        //  OneControl --server
        // or
        //  OneControl --type server
        // to specify that this machine will act as the server? I think the latter is more verbose and therefore would confuse users less.

        // Register arguments
        args::ArgumentParser parser("OneControl - Software KVM Switch", "The project source code is available at: https://github.com/szymonj99/OneControl");
        args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
        args::Group group(parser, "This group can have any number of options selected:", args::Group::Validators::DontCare);
        args::Group exclusive(parser, "This group is exclusive: (at most one option)", args::Group::Validators::AtMostOne);

        //TODO: Add args::CompletionFlag to allow for auto-completion when running under Bash.
        // https://github.com/Taywee/args/blob/master/examples/bash_completion.sh

        args::ValueFlag<std::uint16_t> port(group, "port", "The port on which to connect to/listen on. If no port is set, it will use the default port of " + std::to_string(oc::kDefaultPort), {'p', "port"}, 40480);
        args::ValueFlag<std::string> type(group, "type", "Set this machine to act as either the server or the client. Use values [s, server] or [c, client].", {'t', "type"}, "server");
        args::ValueFlag<std::string> serverIP(group, "serverIP", "The IP to connect to when running OneControl as a Client. This option does nothing when OneControl is run as the server", {'s', "server", "server-ip"}, "127.0.0.1");

        // Parse and handle errors
        try
        {
            parser.ParseCLI(argc, argv);
        }
        catch (const args::Help&)
        {
            std::cout << parser;
            return oc::ReturnCode::ShowHelpMenu;
        }
        catch (const args::ParseError& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << parser;
            return oc::ReturnCode::ParserError;
        }
        catch (args::ValidationError& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << parser;
            return oc::ReturnCode::ValidationError;
        }
        // Set runtime values
        if (type)
        {
            // TODO: Figure out a way to convert the string to lowercase
            // We want to compare 'c', "client", 's', and "server".
            // Anything else would be considered a typo.
            // However, the issue arises when we use unicode.
            // Let's follow the practices defined by https://utf8everywhere.org/
            // We treat std::string as being unicode UTF-8
            // TODO: Future work here might involve the UTF8-CPP library.
            std::string lowerType = std::string("", args::get(type).length());
            std::transform(args::get(type).begin(), args::get(type).end(), lowerType.begin(), ::tolower);
            if (lowerType == "c" || lowerType == "client")
            {
                oc::RuntimeGlobals::isClient = true;
                oc::RuntimeGlobals::isServer = !oc::RuntimeGlobals::isClient;
                std::cout << "This machine will be the client\n";
            }
            // Could use <locale> here
            else if (lowerType == "s" || lowerType == "server")
            {
                oc::RuntimeGlobals::isClient = false;
                oc::RuntimeGlobals::isServer = !oc::RuntimeGlobals::isClient;
                std::cout << "This machine will be the server\n";
            }
            else
            {
                std::cout << "Unknown type: " << args::get(type) << "\n";
                std::cout << "Use either s or server, or c or client.\n";
                return oc::ReturnCode::InvalidTypeString;
            }
        }
        if (oc::RuntimeGlobals::isClient && serverIP)
        {
            if (sf::IpAddress(args::get(serverIP)) == sf::IpAddress::None)
            {
                std::cout << "Incorrect IP address format: " << args::get(serverIP) << "\n";
                return oc::ReturnCode::InvalidIPAddress;
            }
            else
            {
                oc::RuntimeGlobals::customServerIP = true;
                oc::RuntimeGlobals::serverIP = sf::IpAddress(args::get(serverIP));
                std::cout << "Connecting to server IP: " << oc::RuntimeGlobals::serverIP.toString() << "\n";
            }
        }
        if (port)
        {
            oc::RuntimeGlobals::customPort = true;
            oc::RuntimeGlobals::port = args::get(port);
            std::cout << "Running on port: " << oc::RuntimeGlobals::port << "\n";
        }
        return oc::ReturnCode::Success;
    }
}
