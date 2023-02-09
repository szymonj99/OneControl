#ifdef OS_ERROR
#error Platform is not known/supported.
#endif

#include <OneControl/OneControl.h>

int main(int argc, char* argv[])
{
    if (!oc::ParseArguments(argc, argv)) { return 1; }
    std::make_unique<oc::OneControl>()->Start();
	fmt::print("Program finished. Press enter to exit.");
	std::cin.get();
	return 0;
}
