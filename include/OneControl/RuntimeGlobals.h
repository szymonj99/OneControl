#pragma once

#include <cstdint>

#include <SFML/Network/IpAddress.hpp>

namespace oc::RuntimeGlobals
{
	// TODO: As these can be accessed by multiple threads, wrap these in a mutex. Also add get and set functions for ease of use.
	inline bool isServer = false;
	inline bool isClient = false;
	inline bool customPort = false;
	inline uint16_t port = 0;

	// Let's create both mouse and keyboard interfaces, and allow the user to opt-out using a flag such as --no-keyboard or --no-mouse. :thumbs_up:
	inline bool mouseEnabled = true;
	inline bool keyboardEnabled = true;

	inline bool customServerIP = false;
	inline sf::IpAddress serverIP = sf::IpAddress::None;

    inline bool sendToClient = true;
	inline bool receiveFromServer = true;
}
