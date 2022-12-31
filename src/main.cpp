#ifdef OS_ERROR
#error Platform is not known/supported.
#endif

#include <fmt/core.h>

#include <OneControl/OneControl.h>
#include <OneControl/RuntimeGlobals.h>

int main(int argc, char* argv[])
{
	//oc::ParseArgs(argc, argv);
	std::make_unique<oc::OneControl>()->Start();
	fmt::print("Program finished. Press enter to exit.");
	std::cin.get();
	return 0;
}
