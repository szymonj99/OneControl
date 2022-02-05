#ifdef _WIN32

#include "MessageTimer.h"

oc::MessageTimer::MessageTimer(const uint32_t milliseconds, const oc::eThreadMessages messageToSend, const DWORD threadID)
{
	m_Duration = std::chrono::milliseconds{ milliseconds };
	m_ThreadID = threadID;
	m_Message = messageToSend;

	m_pThread = std::make_unique<std::thread>([&] { Function(); });
}

void oc::MessageTimer::Function()
{
	while (!m_EndThread)
	{
		std::this_thread::sleep_for(m_Duration);

		if (m_EndThread || PostThreadMessage(m_ThreadID, static_cast<UINT>(m_Message), 0, (LPARAM) oc::eThreadMessages::KeepAlive) == 0)
		{
			m_EndThread = true;
			return;
		}
	}
}

oc::MessageTimer::~MessageTimer()
{
	m_pThread->~thread();
}

#endif
