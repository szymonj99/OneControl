#ifdef OS_WINDOWS

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
	while (!m_StopRequested)
	{
		// To make the program feel more responsive, we check if stop_requested more often than once every few seconds.
		const auto kDivisions = 5;
		for (auto i = 0; i < kDivisions && !m_StopRequested; i++)
		{
			if (m_StopRequested)
			{
				return;
			}

			std::this_thread::sleep_for(m_Duration / kDivisions);
		}

		if (m_StopRequested || PostThreadMessage(m_ThreadID, static_cast<UINT>(m_Message), 0, (LPARAM) oc::eThreadMessages::KeepAlive) == 0)
		{
			return;
		}
	}
}

oc::MessageTimer::~MessageTimer()
{
	m_StopRequested = true;
	m_pThread->join();
}

#endif
