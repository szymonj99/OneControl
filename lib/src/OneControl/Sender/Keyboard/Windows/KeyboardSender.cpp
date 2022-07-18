#ifdef OS_WINDOWS

#include <OneControl/KeyboardSender.h>

oc::KeyboardSender::KeyboardSender()
{
	m_StartHook();
}

void oc::KeyboardSender::m_StartHook()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.
	m_pHook = SetWindowsHookEx(WH_KEYBOARD_LL, &oc::KeyboardSender::HookProc, 0, 0);
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
		const auto hookStruct = *(KBDLLHOOKSTRUCT*)lParam;
		oc::Input input;
		input.type = oc::eInputType::Keyboard;
		input.keyboard.key = hookStruct.vkCode;
		input.keyboard.state = hookStruct.flags;

		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Keyboard), 0, reinterpret_cast<LPARAM>(&input));

		return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}


oc::Input oc::KeyboardSender::GetHookData()
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
