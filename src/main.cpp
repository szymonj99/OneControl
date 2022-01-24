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

#include "Program/OneControl.h"
#include "Helpers/StaticHelpers.h"

int main(int argc, char* argv[])
{
	oc::ParseFlags(argc, argv);
	std::make_unique<oc::OneControl>()->Start();
	fmt::print("Program finished. Press enter to exit.");
	std::cin.get();
	return 0;
}
