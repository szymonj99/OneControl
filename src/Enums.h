#pragma once

namespace oc
{
	enum class eMachineState
	{
		Server,
		Client
	};

	enum class eEvents
	{
		MouseMoveDown,
		MouseMoveUp,
		KeyPressDown,
		KeyPressUp,
		ScrollMoveUp,
		ScrollMoveDown,
		ScrollPressUp,
		ScrollPressDown
	};
}