#pragma once

namespace oc
{
	/**
	 * A state of the current machine. Used to specify if the current machine is a client or a server.
	 */
	enum class eMachineState
	{
		Uninitialised = 0,
		Server,
		Client
	};
}
