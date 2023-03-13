#pragma once

#include <cstdint>

#include <SFML/Network/IpAddress.hpp>

namespace oc::RuntimeGlobals
{
	// TODO: As these can be accessed by multiple threads, wrap these in a mutex. Also add get and set functions for ease of use.
	inline std::atomic<bool> isServer = false;
	inline std::atomic<bool> isClient = false;
	inline std::atomic<bool> customPort = false;
	inline uint16_t port = 0;

	inline bool customServerIP = false;
	inline sf::IpAddress serverIP = sf::IpAddress::None;
}
