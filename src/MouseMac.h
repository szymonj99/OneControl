#pragma once

#ifdef __APPLE__

#include <cstdint>
#include <utility>

#include "MouseMac.h"

namespace oc
{
	class MouseMac
	{
	public:
		void MoveMouseTo(const int32_t x, const int32_t y);
		std::pair<int32_t, int32_t> GetMousePosition();
	};
}

#endif
