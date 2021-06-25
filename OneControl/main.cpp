#pragma once

#define UNICODE 1
#ifdef _WIN32
// Windows 10
#define _WIN32_WINNT 0x0A00
#define WINDOWS 1
#elif __APPLE__
#define APPLE 1
#elif __linux__
#define LINUX 1
#else
#error Platform is not known/supported.
#endif

#include "OneControl.h"

int main()
{
	{
		std::wcout << L"OneControl is starting.\n";
		auto oneControl = std::make_unique<oc::OneControl>();
		oneControl->Start();
	}
	std::wcout << L"Program finished. Press enter to exit.";
	std::wcin.get();
	return 0;
}