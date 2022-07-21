#pragma once

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

#include <OneControl/Constants.h>

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