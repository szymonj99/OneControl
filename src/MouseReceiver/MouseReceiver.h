#pragma once

#define UNICODE 1

#ifdef _WIN32
#include <Windows.h>
#endif

#include "../Helpers/Constants.h"

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
