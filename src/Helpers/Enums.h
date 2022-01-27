#pragma once

#define UNICODE 1

namespace oc
{
	enum class eMachineState
	{
		Server = 1,
		Client = 2
	};

	enum class eInputType
	{
		Mouse = 0,
		Keyboard
	};

#ifdef _WIN32
#define WM_USER 0x0400
#define WM_APP  0x8000
	enum class eThreadMessages
	{
		Empty = WM_APP + 1,
		Mouse,
		Keyboard
	};
#endif
}
