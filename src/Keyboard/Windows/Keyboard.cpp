#ifdef _WIN32

#include "../Keyboard.h"

bool oc::Keyboard::SendToClient = true;
std::deque<oc::KeyboardPair> oc::Keyboard::Queue = std::deque<oc::KeyboardPair>();
std::mutex oc::Keyboard::QueueMutex;

LRESULT CALLBACK oc::Keyboard::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(0, nCode, wParam, lParam);
	}

	// if return 1, the mouse movements will NOT be passed along further
	// if return CallNextHookEx(0, nCode, wParam, lParam), the movement will be passed along further
	if (oc::Keyboard::SendToClient)
	{
		// Process message
		const auto kb = (KBDLLHOOKSTRUCT*)lParam;
		oc::KeyboardPair keyPress = { kb->vkCode, kb->flags };
		std::unique_lock<std::mutex> lock(oc::Keyboard::QueueMutex);
		oc::Keyboard::Queue.push_back(keyPress);
		lock.unlock();
		PostThreadMessage(GetCurrentThreadId(), static_cast<UINT>(oc::eThreadMessages::Keyboard), 0, 0);
		return 1;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

void oc::Keyboard::StartHook()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); // Force the system to create a message queue.

	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
	m_pHook = SetWindowsHookEx(WH_KEYBOARD_LL, oc::Keyboard::HookProc, 0, 0);

	sf::Packet pkt;
	while (GetMessage(&msg, 0, static_cast<UINT>(oc::eThreadMessages::Keyboard), static_cast<UINT>(oc::eThreadMessages::Keyboard)) > 0)
	{
		// Can try replacing 'oc::Keyboard::Queue.at' with 'oc::Keyboard::Queue[]'.
		std::unique_lock<std::mutex> lock(QueueMutex);
		pkt << static_cast<oc::InputInt>(oc::eInputType::Keyboard) << oc::Keyboard::Queue.at(0).first << oc::Keyboard::Queue.at(0).second;
		oc::Keyboard::Queue.pop_front();
		lock.unlock();
		if (!m_pServer->SendPacket(pkt))
		{
			return;
		}
		pkt.clear();
	}
}

void oc::Keyboard::EndHook()
{
	if (m_pHook)
	{
		UnhookWindowsHookEx(m_pHook);
	}
}

oc::Keyboard::Keyboard()
{
	// I will make use of this later.
}

oc::Keyboard::~Keyboard()
{
	EndHook();
}

#endif
