#pragma once

#include <cstdint>
#include <utility>

namespace oc
{
	/**
	 * Used for specifying which version of the software is being used to connected machines.
	 */
	using VersionInt = uint16_t; // Versioning scheme type

	/**
	 * The default port on which the application will be running.
	 */
	constexpr uint16_t kDefaultPort = 40480;
}
