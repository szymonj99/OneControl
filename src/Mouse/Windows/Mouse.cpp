#ifdef _WIN32

#include "../Mouse.h"

bool oc::Mouse::SendToClient = true;
std::deque<oc::MousePair> oc::Mouse::Queue = std::deque<oc::MousePair>();
std::mutex oc::Mouse::QueueMutex;

LRESULT CALLBACK oc::Mouse::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(0, nCode, wParam, lParam);
	}

	// if return 1, the mouse movement will be captured and not passed to further windows.
	// if return CallNextHookEx(0, nCode, wParam, lParam), the movement will be passed further to other windows.
	if (oc::Mouse::SendToClient)
	{
		// Process message
		const auto ms = (MSLLHOOKSTRUCT*)lParam;
		const oc::MousePair data = { ms->pt.x, ms->pt.y };
		std::unique_lock<std::mutex> lock(oc::Mouse::QueueMutex);
		oc::Mouse::Queue.push_back(data);
		lock.unlock();
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Mouse), 0, 0);
		return CallNextHookEx(0, nCode, wParam, lParam); //return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

oc::Mouse::Mouse()
{
	// I will make use of this later.
}

oc::Mouse::~Mouse()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

void oc::Mouse::StartHook()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.

	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
	m_pHook = SetWindowsHookEx(WH_MOUSE_LL, oc::Mouse::HookProc, 0, 0);

	sf::Packet pkt;
	while (GetMessage(&msg, 0, static_cast<UINT>(oc::eThreadMessages::Mouse), static_cast<UINT>(oc::eThreadMessages::Mouse)) > 0)
	{
		if (msg.message == static_cast<UINT>(oc::eThreadMessages::Mouse))
		{
			// Can try replacing 'oc::Mouse::Queue.at' with 'oc::Mouse::Queue[]'.
			std::unique_lock<std::mutex> lock(oc::Mouse::QueueMutex);
			pkt << static_cast<oc::InputInt>(oc::eInputType::Mouse) << oc::Mouse::Queue.at(0).first << oc::Mouse::Queue.at(0).second;
			oc::Mouse::Queue.pop_front();
			lock.unlock();
			if (!m_pServer->SendPacket(pkt))
			{
				return;
			}
			pkt.clear();
		}
	}
}

void oc::Mouse::EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

void oc::Mouse::MoveMouseTo(const  oc::MouseInt x, const  oc::MouseInt y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(input));
}

void oc::Mouse::MoveMouseRelative(const  oc::MouseInt x, const  oc::MouseInt y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(input));
}

//This returns the cursor position relative to screen coordinates. Call ScreenToClient to map to window coordinates.
oc::MousePair oc::Mouse::GetMousePosition()
{
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	return { mousePoint.x, mousePoint.y };
}

#endif
