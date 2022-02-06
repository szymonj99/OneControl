#ifdef _WIN32

#include "../MouseReceiver.h"

#define DEBUG_MOUSE_MOVE 1

oc::MouseReceiver::MouseReceiver(){}

oc::MouseReceiver::~MouseReceiver(){}

void oc::MouseReceiver::MoveMouseTo(const oc::MouseInt x, const oc::MouseInt y)
{
#ifdef DEBUG_MOUSE_MOVE
	INPUT input{};
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(input));
#endif
}

void oc::MouseReceiver::MoveMouseRelative(const oc::MouseInt x, const oc::MouseInt y)
{
#ifdef DEBUG_MOUSE_MOVE
	INPUT input{};
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(input));
#endif
}

#endif
