#pragma once

namespace oc
{
	// TODO: Add a ReturnCode enum here.
	// TODO: Also add this to OneLibrary.

	// TODO: Lower the size of this enum
	/**
	 * A state of the current machine. Used to specify if the current machine is a client or a server.
	 */
	enum class eMachineState : uint32_t
	{
		Uninitialised = 0,
		Server,
		Client
	};
}
