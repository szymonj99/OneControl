#ifdef __APPLE__

#include "../Mouse.h"

oc::Mouse::Mouse()
{

}

oc::Mouse::~Mouse()
{

}

void oc::Mouse::MoveMouseTo(const oc::MouseInt x, const oc::MouseInt y)
{

}

void oc::Mouse::MoveMouseRelative(const oc::MouseInt x, const oc::MouseInt y)
{

}

oc::MousePair oc::Mouse::GetMousePosition()
{
	return { 0, 0 };
}

#endif
