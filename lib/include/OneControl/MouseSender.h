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

#include <OneControl/Constants.h>
#include <OneControl/Enums.h>
#include <OneControl/Queue.h>
#include <OneControl/Input.h>

namespace oc
{
	class MouseSender
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
		MouseSender();
		~MouseSender();

#ifdef OS_WINDOWS
		// Note: This will block until an input is received.
		oc::Input GetHookData();

		// Static variables!
		static inline bool SendToClient = true;
		// This is what's called every time a low-level mouse event happens.
		static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
#elif OS_LINUX

#elif OS_APPLE
#endif
	};
}
