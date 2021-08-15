#pragma once

#include <memory>

#ifdef _WIN32
#include "MouseWin.h"
#elif __APPLE__
#include "MouseMac.h"
#elif __linux__
#include "MouseLinux.h"
#else
#error Platform is not known/supported.
#endif

namespace oc
{
	class IMouse
	{
	private:
#ifdef _WIN32
		std::unique_ptr<MouseWin> m_pMouseInterface = std::make_unique<MouseWin>();
#elif __APPLE__
		std::unique_ptr<MouseMac> m_pMouseInterface = std::make_unique<MouseMac>();
#elif __linux__
		std::unique_ptr<MouseLinux> m_pMouseInterface = std::make_unique<MouseLinux>();
#endif

	public:
		void MoveMouseTo(const int32_t x, const int32_t y);
		std::pair<int32_t, int32_t> GetMousePosition();
	};
}
