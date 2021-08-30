#define UNICODE 1
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

#include "./Program/OneControl.h"

int main()
{
	std::cout << "OneControl is starting.\n";
	std::make_unique<oc::OneControl>()->Start();
	std::cout << "Program finished. Press enter to exit.";
	std::cin.get();
	return 0;
}