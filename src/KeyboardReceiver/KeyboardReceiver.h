#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include "../Helpers/Constants.h"

namespace oc
{
	class KeyboardReceiver
	{
	public:
		KeyboardReceiver();
		~KeyboardReceiver();
		void KeyPress(const oc::KeyboardInt, const oc::KeyboardInt);
	};
}
