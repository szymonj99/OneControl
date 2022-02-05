#pragma once

#define UNICODE 1

namespace oc
{
	enum class eMachineState : uint32_t
	{
		Server = 1,
		Client
	};

	enum class eInputType : uint32_t
	{
		KeepAlive = 1,
		Mouse,
		Keyboard
	};

#ifdef _WIN32
#define WM_USER 0x0400
#define WM_APP  0x8000
	enum class eThreadMessages : uint32_t
	{
		Empty = WM_APP + 1,
		Mouse,
		Keyboard,
		KeepAlive
	};
#endif
}
