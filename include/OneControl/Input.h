#pragma once

#include <SFML/Network/Packet.hpp>

#include <OneLibrary/Input.h>
#include <OneControl/Packet.h>

namespace oc
{
	// Let's maybe serialize this in the future.
	sf::Packet& operator<<(sf::Packet& packet, const ol::Input& input);
	sf::Packet& operator>>(sf::Packet& packet, ol::Input& input);
}
