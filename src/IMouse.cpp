#include "IMouse.h"

void oc::IMouse::MoveMouseTo(const int32_t x, const int32_t y)
{
	m_pMouseInterface->MoveMouseTo(x, y);
}

std::pair<int32_t, int32_t> oc::IMouse::GetMousePosition()
{
	return m_pMouseInterface->GetMousePosition();
}