#pragma once

#define UNICODE 1

#include <cstdint>
#include <utility>

namespace oc
{
	using InputInt = int32_t; // Type of packet. e.g. Mouse, Keyboard, Keep Alive.
	using MouseInt = int32_t; // Mouse co-ordinate
	using MousePair = std::pair<MouseInt, MouseInt>; // Pair of mouse movement co-ordinates
	using KeyboardInt = uint32_t; // Keyboard key flag (pressed, not pressed etc.) or keycode
	using KeyboardPair = std::pair<KeyboardInt, KeyboardInt>; // A pair of the key and its state
	using VersionInt = uint32_t; // Versioning scheme type

	const uint16_t kPort = 40480;
}
