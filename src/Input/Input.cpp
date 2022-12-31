#include <OneControl/Input.h>

namespace oc
{
	// Let's maybe serialize this in the future.
	sf::Packet& operator<<(sf::Packet& packet, const ol::Input& input)
	{
		return packet << static_cast<ol::InputInt>(input.type) << input.mouse.x << input.mouse.y << input.mouse.scroll << input.keyboard.key << input.keyboard.state;
	}

	sf::Packet& operator>>(sf::Packet& packet, ol::Input& input)
	{
		// Gotta handle the input type enum. Yuck.
		ol::InputInt inputType;
		packet >> inputType;
		input.type = static_cast<ol::eInputType>(inputType);

		return packet >> input.mouse.x >> input.mouse.y >> input.mouse.scroll >> input.keyboard.key >> input.keyboard.state;
	}
}
