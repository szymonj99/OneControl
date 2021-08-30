#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <hidusage.h>
#elif __linux__
#include <linux/uinput.h>
#include <linux/input.h>
#include <linux/ioctl.h>
#include <iostream>
#elif __APPLE__

#endif

#include <memory>
#include <iostream>
#include <utility>
#include <cstdint>
#include <thread>

namespace oc
{
	class Mouse
	{
	private:
#ifdef _WIN32
		std::unique_ptr<RAWINPUTDEVICE> m_pRawMouseInput = std::make_unique<RAWINPUTDEVICE>();
#elif __linux__

#elif __APPLE__
#endif

	public:
		Mouse();
		~Mouse();
		void MoveMouseTo(const int32_t x, const int32_t y);
		void MoveMouseRelative(const int32_t x, const int32_t y);
		std::pair<int32_t, int32_t> GetMousePosition();
		// Used to pass relative mouse movement data from mouse procedure to server.
		static std::pair<int32_t, int32_t> RelativeMouseMovement;

#ifdef _WIN32
		// Static variables!

		// This is what's called every time a raw input event happens.
		static LRESULT RawInputMouseProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static WNDCLASS RawInputWindowClass;
		// Used when registering raw input device.
		static HWND RawInputMessageWindow;
#elif __linux__

#elif __APPLE__
#endif
	};
}
