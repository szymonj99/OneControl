#ifdef _WIN32
// Windows 10
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#define WINDOWS 1
#elif __APPLE__
#define APPLE 1
#elif __linux__
#define LINUX 1
#else
#error Platform is not known/supported.
#endif

#define UNICODE 1

#include <fmt/core.h>
#include <argh.h>

#include "Program/OneControl.h"
#include "Helpers/ParsedFlags.h"

int main(int argc, char* argv[])
{
	// if argc == 1, then we didn't pass in any arguments to parse.
	if (argc > 1)
	{
		argh::parser parser;
		parser.add_params({ "-p", "--port", "--s", "--server", "-t", "--type" });
		parser.parse(argc, argv);

		if (parser["h"] || parser["help"])
		{
			fmt::print("Welcome to OneControl.\n");
			fmt::print("\n");
			fmt::print("OPTIONS:\n");
			fmt::print("\n");
			fmt::print("    -h, --help                   Show the help banner.\n");
			fmt::print("    -p, --port <port>            The port on which to connect to/listen on.\n");
			fmt::print("    -s, --server <ip>            The port on which to connect to/listen on.\n");
			fmt::print("    -t, --type <client/server>   Set this machine to act as either the server or the client.\n");

			return 0;
		}

		fmt::print("Port: " + parser("port").str());
		int i = 0;

		if (parser["p"] || parser["port"])
		{
			fmt::print(parser("port").str());
			return 0;
		}
	}

	fmt::print("OneControl is starting.\n");
	std::make_unique<oc::OneControl>()->Start();
	fmt::print("Program finished. Press enter to exit.");
	std::cin.get();
	return 0;
}
