#pragma once

#ifdef OS_WINDOWS
#include <Windows.h>
#include <hidusage.h>
#elif OS_LINUX
#include <linux/uinput.h>
#include <linux/input.h>
#include <linux/ioctl.h>
#elif OS_APPLE
#endif

#include <memory>
#include <iostream>
#include <utility>
#include <cstdint>
#include <thread>

#include <fmt/core.h>
#include <fmt/color.h>

#include "../Helpers/Constants.h"
#include "../Helpers/Enums.h"

namespace oc
{
	class KeyboardSender
	{
	private:
#ifdef OS_WINDOWS
		HHOOK m_pHook = nullptr;
		void m_StartHook();
		void m_EndHook();
#elif OS_LINUX
#elif OS_APPLE
#endif

	public:
		KeyboardSender();
		~KeyboardSender();

#ifdef OS_WINDOWS
		oc::KeyboardPair GetHookData();

		// Static variables!

		static bool SendToClient;
		// This is what's called every time a raw input event happens.
		static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
#elif OS_LINUX
#elif OS_APPLE
#endif
	};
}
