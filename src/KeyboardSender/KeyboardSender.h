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
	class KeyboardSender
	{
	private:
#ifdef _WIN32
		HHOOK m_pHook = nullptr;
#elif __linux__

#elif __APPLE__
#endif

	public:
		KeyboardSender();
		~KeyboardSender();

#ifdef _WIN32
		void StartHook();
		oc::KeyboardPair GetHookData();
		void EndHook();

		// Static variables!

		static bool SendToClient;
		// This is what's called every time a raw input event happens.
		static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
#elif __linux__

#elif __APPLE__
#endif
	};
}
