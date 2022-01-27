#ifdef _WIN32

#include "../MouseSender.h"

bool oc::MouseSender::SendToClient = true;

oc::MouseSender::MouseSender() {};

oc::MouseSender::~MouseSender()
{
	EndHook();
}

LRESULT CALLBACK oc::MouseSender::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(0, nCode, wParam, lParam);
	}

	// if return 1, the mouse movement will be captured and not passed to further windows.
	// if return CallNextHookEx(0, nCode, wParam, lParam), the movement will be passed further to other windows.
	if (oc::MouseSender::SendToClient)
	{
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Mouse), 0, lParam);
		return CallNextHookEx(0, nCode, wParam, lParam); //return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

void oc::MouseSender::StartHook()
{
	MSG msg;
	PeekMessage(&msg, 0, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.

	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
	m_pHook = SetWindowsHookEx(WH_MOUSE_LL, oc::MouseSender::HookProc, 0, 0);
}

oc::MousePair oc::MouseSender::GetHookData()
{
	MSG msg;
	// Not as pretty as I would like it to be.
	if (GetMessage(&msg, 0, WM_TIMER, static_cast<UINT>(oc::eThreadMessages::Mouse)) > 0)
	{
		if (msg.message == static_cast<UINT>(oc::eThreadMessages::Mouse))
		{
			if (msg.lParam != 0)
			{
				// msg.lParam is going to be whatever we set as the last argument in PostThreadMessage
				const auto ms = (MSLLHOOKSTRUCT*) msg.lParam;
				return { ms->pt.x, ms->pt.y };
			}
			else
			{
				return oc::MousePair(INT32_MIN, INT32_MIN);
			}
		}
		else if (msg.message == WM_TIMER)
		{
			fmt::print("Mouse timer.\n");
			return oc::MousePair(INT32_MIN, INT32_MIN);
		}
		else if (msg.message == WM_QUIT)
		{
			fmt::print("Mouse hook quitting.\n");
			return oc::MousePair(INT32_MIN, INT32_MIN);
		}
	}
	return oc::MousePair(INT32_MIN, INT32_MIN);
}

void oc::MouseSender::EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

#endif
