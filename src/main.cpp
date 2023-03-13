#ifdef OS_ERROR
    #error Platform is not known/supported.
#endif

#include <OneControl/OneControl.h>

int main(const int argc, const char* argv[])
{
    if (oc::ParseArguments(argc, argv) != oc::ReturnCode::Success) { return static_cast<uint32_t>(oc::ReturnCode::CheckParserOutput); }
    std::make_unique<oc::OneControl>()->Start();
    std::cout << "Program finished successfully" << std::endl;
	return static_cast<uint32_t>(oc::ReturnCode::Success);
}
