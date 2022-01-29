#ifdef _WIN32

#include "MessageTimer.h"

oc::MessageTimer::MessageTimer(const uint32_t milliseconds, const oc::eThreadMessages messageToSend, const DWORD threadID, const uint32_t divisions)
{
	m_Duration = std::chrono::milliseconds{ milliseconds };
	m_ThreadID = threadID;
	m_Message = messageToSend;
	m_iDivisions = divisions;

	m_pThread = std::make_unique<std::thread>([&] { Function(); });
}

void oc::MessageTimer::Function()
{
	while (!m_EndThread)
	{
		// Divide this into intervals, so ending the program is more responsive.
		// Could this, or ~MesageTimer be replaced by something like `wait_until`?
		for (uint8_t i = 0; i < m_iDivisions && !m_EndThread; i++)
		{
			std::this_thread::sleep_for(m_Duration / m_iDivisions);
		}

		if (m_EndThread || PostThreadMessage(m_ThreadID, static_cast<UINT>(m_Message), 0, (LPARAM) oc::eThreadMessages::KeepAlive) == 0)
		{
			m_EndThread = true;
			return;
		}
	}
}

oc::MessageTimer::~MessageTimer()
{
	m_EndThread = true;
	while (!m_pThread->joinable())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds{ 10 });
	}
	m_pThread->join();
}

#endif
