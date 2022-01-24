#ifdef __APPLE__

#include "../KeyboardReceiver.h"

oc::KeyboardReceiver::KeyboardReceiver() {}

oc::KeyboardReceiver::~KeyboardReceiver() {}

void oc::KeyboardReceiver::KeyPress(const oc::KeyboardInt kScanCode, const oc::KeyboardInt kFlags)
{
#ifdef DEBUG_KEYBOARD_INPUT
#endif
}

#endif