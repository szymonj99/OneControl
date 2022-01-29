#pragma once

#define UNICODE 1

#ifdef _WIN32
#include <Windows.h>
#include <hidusage.h>
#include <deque>
#elif __linux__
#include <linux/uinput.h>
#include <linux/input.h>
#include <linux/ioctl.h>
#elif __APPLE__

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
	class MouseSender
	{
	private:
#ifdef _WIN32
		HHOOK m_pHook = nullptr;
		void m_StartHook();
		void m_EndHook();
#elif __linux__

#elif __APPLE__
#endif

	public:
		MouseSender();
		~MouseSender();

#ifdef _WIN32
		oc::MousePair GetHookData();

		// Static variables!
		static bool SendToClient;
		// This is what's called every time a low-level mouse event happens.
		static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
#elif __linux__

#elif __APPLE__
#endif
	};
}
