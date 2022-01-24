#ifdef _WIN32
#define WINDOWS 1
#define OS_WINDOWS 1
#elif __APPLE__
#define APPLE 1
#define OS_APPLE 1
#elif __linux__
#define LINUX 1
#define OS_LINUX 1
#else
#error Platform is not known/supported.
#endif

#ifdef OS_WINDOWS
// Windows 10
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#define WINDOWS 1
#elif OS_APPLE
#elif OS_LINUX
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
