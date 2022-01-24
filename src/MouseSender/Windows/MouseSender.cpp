#ifdef _WIN32

#include "../MouseSender.h"

bool oc::MouseSender::SendToClient = true;
std::deque<oc::MousePair> oc::MouseSender::Queue = std::deque<oc::MousePair>();
std::mutex oc::MouseSender::QueueMutex;

oc::MouseSender::MouseSender() {} // I will make use of this later.

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
		// Process message
		const auto ms = (MSLLHOOKSTRUCT*)lParam;
		const oc::MousePair data = { ms->pt.x, ms->pt.y };
		std::unique_lock<std::mutex> lock(oc::MouseSender::QueueMutex);
		oc::MouseSender::Queue.push_back(data);
		lock.unlock();
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Mouse), 0, 0);
		return CallNextHookEx(0, nCode, wParam, lParam); //return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

void oc::MouseSender::StartHook()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.

	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
	m_pHook = SetWindowsHookEx(WH_MOUSE_LL, oc::MouseSender::HookProc, 0, 0);

	ProcessHook();
}

void oc::MouseSender::ProcessHook()
{
	MSG msg;
	sf::Packet pkt;
	while (GetMessage(&msg, 0, static_cast<UINT>(oc::eThreadMessages::Mouse), static_cast<UINT>(oc::eThreadMessages::Mouse)) > 0)
	{
		// Can try replacing 'oc::Mouse::Queue.at' with 'oc::Mouse::Queue[]'.
		std::unique_lock<std::mutex> lock(oc::MouseSender::QueueMutex);
		pkt << static_cast<oc::InputInt>(oc::eInputType::Mouse) << oc::MouseSender::Queue.at(0).first << oc::MouseSender::Queue.at(0).second;
		oc::MouseSender::Queue.pop_front();
		lock.unlock();
		if (!m_pServer->SendPacketToClient(pkt))
		{
			EndHook();
			return;
		}
		pkt.clear();
	}
}

void oc::MouseSender::EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

#endif
