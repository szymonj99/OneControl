#pragma once

#ifdef __linux__

#include <cstdint>
#include <utility>

#include <curses.h>

namespace oc
{
	class MouseLinux
	{
	public:
		void MoveMouseTo(const int32_t x, const int32_t y);
		std::pair<int32_t, int32_t> GetMousePosition();
	};
}

#endif
