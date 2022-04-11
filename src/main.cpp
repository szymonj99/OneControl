#ifdef OS_ERROR
#error Platform is not known/supported.
#endif

#include <fmt/core.h>

#include "Program/OneControl.h"

int main(int argc, char* argv[])
{
	std::make_unique<oc::OneControl>()->Start();
	fmt::print("Program finished. Press enter to exit.");
	std::cin.get();
	return 0;
}
