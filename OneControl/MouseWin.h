#pragma once

#include <Windows.h>
#include <cstdint>
#include <memory>

namespace oc
{
	class MouseWin
	{
	public:
		void MoveMouseTo(const int32_t x, const int32_t y);
		std::pair<int32_t, int32_t> GetMousePosition();
	};
}
