#ifdef OS_WINDOWS

#include <OneControl/MouseSender.h>

oc::MouseSender::MouseSender()
{
	m_StartHook();
}

void oc::MouseSender::m_StartHook()
{
	MSG msg;
	PeekMessage(&msg, 0, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.
	m_pHook = SetWindowsHookEx(WH_MOUSE_LL, &oc::MouseSender::HookProc, 0, 0);
}

oc::MouseSender::~MouseSender()
{
	m_EndHook();
}

void oc::MouseSender::m_EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

// Note to self:
// This can only be called by Windows when we are in a GetMessage loop.
// Makes sense. Otherwise Windows doesn't have control of our program according to someone that can explain this better than I can, on StackOverflow.
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
		POINT point;
		GetCursorPos(&point);

		const auto hookStruct = *(MSLLHOOKSTRUCT*)lParam;

		oc::Input input;
		input.type = oc::eInputType::Mouse;
		input.mouse.x = hookStruct.pt.x - point.x;
		input.mouse.y = hookStruct.pt.y - point.y;

		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Mouse), 0, reinterpret_cast<LPARAM>(&input));

		return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

oc::Input oc::MouseSender::GetHookData()
{
	MSG msg;
	while (GetMessage(&msg, 0, static_cast<UINT>(oc::eThreadMessages::Mouse), static_cast<UINT>(oc::eThreadMessages::Mouse)) > 0)
	{
		switch (msg.message)
		{
		case static_cast<UINT>(oc::eThreadMessages::Mouse):
		{
			if ((oc::eThreadMessages)msg.lParam == oc::eThreadMessages::KeepAlive)
			{
				auto input = oc::Input();
				input.type = oc::eInputType::KeepAlive;
				return input;
			}

			const auto input = reinterpret_cast<oc::Input*>(msg.lParam);

			// Let's abort when something went wrong.
			if (input == nullptr)
			{
				auto failed = oc::Input();
				failed.type = oc::eInputType::Failed;
				return failed;
			}

			return *input;
		}

		case WM_QUIT:
		{
			auto input = oc::Input();
			input.type = oc::eInputType::HookStopped;
			return input;
		}
		default:
		{
			break;
		}
		}
	}

	return oc::Input();
}

#endif
