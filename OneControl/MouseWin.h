#pragma once

#include <Windows.h>
#include <cstdint>
#include <memory>
#include <thread>

namespace oc
{
	class MouseWin
	{
	private:
		std::unique_ptr<HHOOK> m_pMouseHook = nullptr;

	public:
		void MoveMouseTo(const int32_t x, const int32_t y);
		std::pair<int32_t, int32_t> GetMousePosition();
	};
}
