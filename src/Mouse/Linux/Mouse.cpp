#ifdef __linux__

#include "../Mouse.h"

oc::Mouse::Mouse()
{

}

oc::Mouse::~Mouse()
{

}

void oc::Mouse::MoveMouseTo(const MouseInt x, const MouseInt y)
{
	
}

void oc::Mouse::MoveMouseRelative(const MouseInt x, const MouseInt y)
{

}

oc::MousePair oc::Mouse::GetMousePosition()
{
	return { 0, 0 };
}

#endif
