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
		const auto ptr = (KBDLLHOOKSTRUCT*)lParam;
		const auto kb = new KBDLLHOOKSTRUCT(*ptr);
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Keyboard), 0, (LPARAM)kb);
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

	while (GetMessage(&msg, 0, static_cast<UINT>(oc::eThreadMessages::Keyboard), static_cast<UINT>(oc::eThreadMessages::Keyboard)) > 0)
	{
		switch (msg.message)
		{
		case static_cast<UINT>(oc::eThreadMessages::Keyboard):
		{
			const auto kb = (KBDLLHOOKSTRUCT*)msg.lParam;

			if (kb == nullptr)
			{
				return oc::KeyboardPair(INT32_MIN, INT32_MIN);
			}

			const auto output = oc::KeyboardPair(kb->vkCode, kb->flags);
			delete (KBDLLHOOKSTRUCT*)msg.lParam;

			return { output.first, output.second };
		}

		case WM_QUIT:
		{
			return oc::KeyboardPair(INT32_MIN, INT32_MIN);
		}

		default:
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
