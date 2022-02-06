#ifdef _WIN32

#include "../KeyboardSender.h"

bool oc::KeyboardSender::SendToClient = true;

oc::KeyboardSender::KeyboardSender()
{
	m_StartHook();
}

void oc::KeyboardSender::m_StartHook()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.
	m_pHook = SetWindowsHookEx(WH_KEYBOARD_LL, oc::KeyboardSender::HookProc, 0, 0);
}

oc::KeyboardSender::~KeyboardSender()
{
	m_EndHook();
}

void oc::KeyboardSender::m_EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
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
		// https://stackoverflow.com/questions/25667226/how-can-i-use-shared-ptr-using-postthreadmessage
		std::unique_ptr<KBDLLHOOKSTRUCT> kbPtr(new KBDLLHOOKSTRUCT(*(KBDLLHOOKSTRUCT*)lParam));

		if (PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Keyboard), 0, reinterpret_cast<LPARAM>(kbPtr.get())))
		{
			// Release the ownership of this smart pointer.
		    // The pointer will be recreated in the GetHookData function, and will gain full ownership.
			kbPtr.release();
		}
		return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
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
			if ((oc::eThreadMessages)msg.lParam == oc::eThreadMessages::KeepAlive)
			{
				return oc::KeyboardPair(INT32_MAX, INT32_MAX);
			}

			// https://stackoverflow.com/questions/25667226/how-can-i-use-shared-ptr-using-postthreadmessage
			std::unique_ptr<KBDLLHOOKSTRUCT> kb(reinterpret_cast<KBDLLHOOKSTRUCT*>(msg.lParam));

			if (kb == nullptr)
			{
				return oc::KeyboardPair(INT32_MIN, INT32_MIN);
			}

			return oc::KeyboardPair(kb->vkCode, kb->flags);
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

#endif
