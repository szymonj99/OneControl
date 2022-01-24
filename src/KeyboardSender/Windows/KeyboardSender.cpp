#ifdef _WIN32

#include "../KeyboardSender.h"

bool oc::KeyboardSender::SendToClient = true;
std::deque<oc::KeyboardPair> oc::KeyboardSender::Queue = std::deque<oc::KeyboardPair>();
std::mutex oc::KeyboardSender::QueueMutex;

oc::KeyboardSender::KeyboardSender() {};

oc::KeyboardSender::~KeyboardSender()
{
	EndHook();
}

LRESULT CALLBACK oc::KeyboardSender::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(0, nCode, wParam, lParam);
	}

	// if return 1, the mouse movements will NOT be passed along further
	// if return CallNextHookEx(0, nCode, wParam, lParam), the movement will be passed along further
	if (oc::KeyboardSender::SendToClient)
	{
		// Process message
		const auto kb = (KBDLLHOOKSTRUCT*)lParam;
		oc::KeyboardPair keyPress = { kb->vkCode, kb->flags };
		std::unique_lock<std::mutex> lock(oc::KeyboardSender::QueueMutex);
		oc::KeyboardSender::Queue.push_back(keyPress);
		lock.unlock();
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Keyboard), 0, 0);
		return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

void oc::KeyboardSender::StartHook()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.

	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
	m_pHook = SetWindowsHookEx(WH_KEYBOARD_LL, oc::KeyboardSender::HookProc, 0, 0);
}

oc::KeyboardPair oc::KeyboardSender::GetHookData()
{
	MSG msg;
	if (GetMessage(&msg, 0, static_cast<UINT>(oc::eThreadMessages::Keyboard), static_cast<UINT>(oc::eThreadMessages::Keyboard)) > 0)
	{
		// Can try replacing 'oc::Mouse::Queue.at' with 'oc::Mouse::Queue[]'.
		return oc::KeyboardSender::Queue.at(0);
	}
	return oc::KeyboardPair(-1, -1);
}

void oc::KeyboardSender::EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

#endif
