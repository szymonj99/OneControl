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
		const auto ptr = (MSLLHOOKSTRUCT*)lParam;
		const auto ms = new MSLLHOOKSTRUCT(*ptr);
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Mouse), 0, (LPARAM)ms);
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

	while (GetMessage(&msg, 0, static_cast<UINT>(oc::eThreadMessages::Mouse), static_cast<UINT>(oc::eThreadMessages::Mouse)) > 0)
	{
		switch (msg.message)
		{
		case static_cast<UINT>(oc::eThreadMessages::Mouse):
		{
			const auto ms = (MSLLHOOKSTRUCT*) msg.lParam;

			if (ms == nullptr)
			{
				return oc::MousePair(INT32_MIN, INT32_MIN);
			}

			const auto output = oc::MousePair(ms->pt.x, ms->pt.y);
			delete (MSLLHOOKSTRUCT*)msg.lParam;

			return { output.first, output.second };
		}

		case WM_QUIT:
		{
			return oc::MousePair(INT32_MIN, INT32_MIN);
		}

		default:
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
