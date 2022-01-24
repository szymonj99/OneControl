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
#include "../Server/Server.h"
#include "../Helpers/Enums.h"

namespace oc
{
	class Server;

	class KeyboardSender
	{
	private:
		oc::Server* m_pServer = nullptr;

#ifdef _WIN32
		HHOOK m_pHook = nullptr;
#elif __linux__

#elif __APPLE__
#endif
	public:
		KeyboardSender();
		~KeyboardSender();
		void SetServer(oc::Server* server);

#ifdef _WIN32
		void StartHook();
		void EndHook();

		// Static variables!

		static bool SendToClient;
		static std::mutex QueueMutex;
		static std::deque<KeyboardPair> Queue;
		// This is what's called every time a raw input event happens.
		static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
#elif __linux__

#elif __APPLE__
#endif
	};
}
