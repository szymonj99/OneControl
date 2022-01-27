#ifdef _WIN32

#include "../KeyboardSender.h"

bool oc::KeyboardSender::SendToClient = true;

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
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Keyboard), 0, lParam);
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
	// Not as pretty as I would like it to be.
	if (GetMessage(&msg, 0, WM_TIMER, static_cast<UINT>(oc::eThreadMessages::Keyboard)) > 0)
	{
		if (msg.message == static_cast<UINT>(oc::eThreadMessages::Keyboard))
		{
			if (msg.lParam != 0)
			{
				// msg.lParam is going to be whatever we set as the last argument in PostThreadMessage
				const auto kb = (KBDLLHOOKSTRUCT*) msg.lParam;
				return { kb->vkCode, kb->flags };
			}
			else
			{
				return oc::KeyboardPair(INT32_MIN, INT32_MIN);
			}
		}
		else if (msg.message == WM_TIMER)
		{
			fmt::print("Keyboard timer.\n");
			return oc::KeyboardPair(INT32_MIN, INT32_MIN);
		}
		else if (msg.message == WM_QUIT)
		{
			fmt::print("Keyboard hook quitting.\n");
			return oc::KeyboardPair(INT32_MIN, INT32_MIN);
		}
	}
	return oc::KeyboardPair(INT32_MIN, INT32_MIN);
}

void oc::KeyboardSender::EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

#endif
