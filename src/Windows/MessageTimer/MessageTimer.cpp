#ifdef _WIN32

#include "MessageTimer.h"

oc::MessageTimer::MessageTimer(const uint32_t milliseconds, const oc::eThreadMessages messageToSend, const DWORD threadID)
{
	m_Duration = std::chrono::milliseconds{ milliseconds };
	m_ThreadID = threadID;
	m_Message = messageToSend;

	const auto postMessageFunction = [&]
	{
		Function();
	};

	m_pThread = std::make_unique<std::thread>(postMessageFunction);
	m_pThread->detach();
}

void oc::MessageTimer::Function()
{
	while (!m_EndThread)
	{
		std::this_thread::sleep_for(m_Duration);
		if (PostThreadMessage(m_ThreadID, static_cast<UINT>(m_Message), 0, 0) == 0)
		{
			std::cout << "Failed to send timer message with Error Code: " << GetLastError() << "\n";
		}
	}
}

oc::MessageTimer::~MessageTimer()
{
	std::cout << "Destroying Timer.\n";
	m_EndThread = true;
	m_pThread->join();
}

#endif
