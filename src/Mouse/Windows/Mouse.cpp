#ifdef _WIN32

#include "../Mouse.h"

std::pair<int32_t, int32_t> oc::Mouse::RelativeMouseMovement = { 0,0 };
WNDCLASS oc::Mouse::RawInputWindowClass;
HWND oc::Mouse::RawInputMessageWindow;

LRESULT oc::Mouse::RawInputMouseProcedure(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, 0, &dwSize, sizeof(RAWINPUTHEADER));
		auto lpb = std::make_shared<BYTE[]>(dwSize);
		if (!lpb || (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.get(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize))
		{
			break;
		}
		const auto raw = (PRAWINPUT)lpb.get();
		// No need to check if raw->header.dwType == RIM_TYPEMOUSE
		// As we have registered the raw input device as a mouse only.
		oc::Mouse::RelativeMouseMovement = { raw->data.mouse.lLastX , raw->data.mouse.lLastY };
		return dwSize;
	}
	case WM_CLOSE:
	{
		std::cout << "Raw input WM_CLOSE received.\n";
		std::cin.get();
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}

oc::Mouse::Mouse()
{
	// Window class
	oc::Mouse::RawInputWindowClass.hInstance = (HINSTANCE)1;
	oc::Mouse::RawInputWindowClass.lpszClassName = "OneControl";
	oc::Mouse::RawInputWindowClass.lpfnWndProc = oc::Mouse::RawInputMouseProcedure;
	RegisterClass(&oc::Mouse::RawInputWindowClass);

	// Create message window:
	// Invisible window that we use to get raw input messages.
	oc::Mouse::RawInputMessageWindow = CreateWindow(oc::Mouse::RawInputWindowClass.lpszClassName, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, oc::Mouse::RawInputWindowClass.hInstance, 0);

	m_pRawMouseInput->usUsagePage = HID_USAGE_PAGE_GENERIC;
	m_pRawMouseInput->usUsage = HID_USAGE_GENERIC_MOUSE;
	// RIDEV_INPUTSINK is used so that we can get messages from the raw input device even when the application is no longer in focus.
	m_pRawMouseInput->dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK; // adds mouse and also ignores legacy mouse messages
	m_pRawMouseInput->hwndTarget = oc::Mouse::RawInputMessageWindow;

	if (!RegisterRawInputDevices(&*m_pRawMouseInput, 1, sizeof(RAWINPUTDEVICE)))
	{
		std::cout << "Registering of raw input devices failed.\n";
		std::cin.get();
		std::exit(-1);
	}
}

oc::Mouse::~Mouse()
{
	m_pRawMouseInput->dwFlags = RIDEV_REMOVE;
	m_pRawMouseInput->hwndTarget = NULL;
	// This acts as unregistering the raw input device: https://gamedev.net/forums/topic/629795-unregistering-raw-input/4970645/
	if (!RegisterRawInputDevices(&*m_pRawMouseInput, 1, sizeof(RAWINPUTDEVICE)))
	{
		std::cout << "Unregistering of raw input devices failed.\n";
		std::cin.get();
		std::exit(-1);
	}
}

void oc::Mouse::MoveMouseTo(const int32_t x, const int32_t y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(input));
}

void oc::Mouse::MoveMouseRelative(const int32_t x, const int32_t y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(input));
}

//This returns the cursor position relative to screen coordinates. Call ScreenToClient to map to window coordinates.
std::pair<int32_t, int32_t> oc::Mouse::GetMousePosition()
{
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	return { mousePoint.x, mousePoint.y };
}

#endif
