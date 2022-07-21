#pragma once

#include <cstdint>

#include <SFML/Network/IpAddress.hpp>

namespace oc
{
	// This namespace will store some runtime configuration that other areas of the code would need.
	// For example, configuration options with `argh!` such as port or server/client choice.
	namespace RuntimeGlobals
	{
		// I wonder...
		// We could store the `argh!` parser object here, or we could store individual options.
		// Let's go with individual options for now to limit dependencies.

		inline bool isServer = false;
		inline bool isClient = false;
		inline uint16_t port = 0;

		// Let's create both mouse and keyboard interfaces, and allow the user to opt-out using a flag such as --no-keyboard or --no-mouse. :thumbs_up:
		inline bool mouseEnabled = true;
		inline bool keyboardEnabled = true;

		inline sf::IpAddress serverIP = sf::IpAddress::None;
	}
}
