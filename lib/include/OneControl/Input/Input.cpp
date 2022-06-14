#include <OneControl/Input/Input.h>

namespace oc
{
	// Let's maybe serialize this in the future.
	sf::Packet& operator<<(sf::Packet& packet, const oc::Input& input)
	{
		return packet << input.mouse.x << input.mouse.y << input.mouse.scroll << input.keyboard.key << input.keyboard.state;
	}

	sf::Packet& operator>>(sf::Packet& packet, oc::Input& input)
	{
		return packet >> input.mouse.x >> input.mouse.y >> input.mouse.scroll >> input.keyboard.key >> input.keyboard.state;
	}
}
