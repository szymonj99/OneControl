#ifdef _WIN32

#include <Windows.h>

#include <cstdint>
#include <chrono>
#include <thread>
#include <memory>
#include <iostream>

#include "../../Helpers/Enums.h"
#include "../../Helpers/Constants.h"

namespace oc
{
	class MessageTimer
	{
	private:
		std::chrono::milliseconds m_Duration = std::chrono::milliseconds{0};
		std::unique_ptr<std::thread> m_pThread = nullptr;
		bool m_EndThread = false;
		oc::eThreadMessages m_Message = oc::eThreadMessages::Empty;
		DWORD m_ThreadID = 0;
		uint32_t m_iDivisions = 0;
		void Function();
		
	public:
		MessageTimer(const uint32_t milliseconds, const oc::eThreadMessages messageToSend, const DWORD threadID, const uint32_t divisions = 10);
		~MessageTimer();
	};
}

#endif
