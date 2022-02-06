#pragma once

namespace oc
{
	enum class eMachineState : uint32_t
	{
		Uninitialised = 1,
		Server,
		Client
	};

	enum class eInputType : uint32_t
	{
		KeepAlive = 1,
		Mouse,
		Keyboard
	};

#ifdef OS_WINDOWS
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
