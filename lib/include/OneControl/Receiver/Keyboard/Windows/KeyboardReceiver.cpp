#ifdef OS_WINDOWS

#include <OneControl/Receiver/Keyboard/KeyboardReceiver.h>

#define DEBUG_KEYBOARD_INPUT 1

oc::KeyboardReceiver::KeyboardReceiver() {}

oc::KeyboardReceiver::~KeyboardReceiver() {}

void oc::KeyboardReceiver::KeyPress(const oc::KeyboardInt kScanCode, const oc::KeyboardInt kFlags)
{
#ifdef DEBUG_KEYBOARD_INPUT
	INPUT input{};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = kScanCode;
	input.ki.dwFlags = kFlags;
	input.ki.time = 0;

	// I'm not 100% sure how we will later implement the scan codes.
	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-keybdinput
	// input.ki.wScan;

	SendInput(1, &input, sizeof(input));
#endif
}

#endif