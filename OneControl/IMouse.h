#pragma once

#ifdef _WIN32
#include "MouseWin.h"
#elif __APPLE__
#include "MouseMac.h"
#elif __linux__
#include "MouseLinux.h"
#else
#error Platform is not known/supported.
#endif

namespace oc
{
	class IMouse
	{

	};
}

