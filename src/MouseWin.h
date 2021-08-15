#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <cstdint>
#include <memory>
#include <thread>

namespace oc
{
	class MouseWin
	{
	private:
		POINT m_mousePoint;

	public:
		void MoveMouseTo(const int32_t x, const int32_t y);
		std::pair<int32_t, int32_t> GetMousePosition();
	};
}

#endif
