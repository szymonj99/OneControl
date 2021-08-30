#ifdef __linux__

#include "../Mouse.h"

std::pair<int32_t, int32_t> oc::Mouse::RelativeMouseMovement = { 0,0 };

oc::Mouse::Mouse()
{

}

oc::Mouse::~Mouse()
{

}

void oc::Mouse::MoveMouseTo(const int32_t x, const int32_t y)
{
	
}

void oc::Mouse::MoveMouseRelative(const int32_t x, const int32_t y)
{

}

std::pair<int32_t, int32_t> oc::Mouse::GetMousePosition()
{
	return { 0, 0 };
}

#endif
