#pragma once

#include <SFML/Network/Packet.hpp>

#include <OneControl/Constants.h>

namespace oc
{
	struct MouseInput
	{
		oc::MouseInt x = 0;
		oc::MouseInt y = 0;
		oc::MouseInt scroll = 0;
	};

	struct KeyboardInput
	{
		oc::KeyboardInt key = 0;
		oc::KeyboardInt state = 0;
	};

	// This class is what will be sent from the Server to the Client.
	// I think for this, adding some tests would be easy.
	// In the future, this could be made ASN.1-compliant.
	struct Input
	{
		oc::MouseInput mouse = oc::MouseInput();
		oc::KeyboardInput keyboard = oc::KeyboardInput();
	};

	// Let's maybe serialize this in the future.
	sf::Packet& operator<<(sf::Packet& packet, const oc::Input& input);
	sf::Packet& operator>>(sf::Packet& packet, oc::Input& input);
}
