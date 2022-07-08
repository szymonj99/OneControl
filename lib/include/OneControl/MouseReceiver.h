#pragma once

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

#include <OneControl/Constants.h>

namespace oc
{
	class MouseReceiver
	{
	public:
		MouseReceiver();
		~MouseReceiver();
		void MoveMouseTo(const oc::MouseInt x, const oc::MouseInt y);
		void MoveMouseRelative(const oc::MouseInt x, const oc::MouseInt y);
	};
}
