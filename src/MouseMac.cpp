#include "MouseMac.h"

#ifdef __APPLE__

void oc::MouseMac::MoveMouseTo(const int32_t x, const int32_t y)
{

}

//This returns the cursor position relative to screen coordinates. Call ScreenToClient to map to window coordinates.
std::pair<int32_t, int32_t> oc::MouseMac::GetMousePosition()
{
	return { 0, 0 };
}

#endif