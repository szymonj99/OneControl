#include "MouseWin.h"

void oc::MouseWin::MoveMouseTo(const int32_t x, const int32_t y)
{	
	SetCursorPos(x, y);
}

//This returns the cursor position relative to screen coordinates. Call ScreenToClient to map to window coordinates.
std::pair<int32_t, int32_t> oc::MouseWin::GetMousePosition()
{
	auto mousePoint = std::make_shared<POINT>();
	GetCursorPos(&*mousePoint);
	return { mousePoint->x, mousePoint->y };
}