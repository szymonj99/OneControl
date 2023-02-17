#include <OneControl/Input.h>

// We need to add more options to this in the future.
namespace oc
{
	// Let's maybe serialize this in the future.
	sf::Packet& operator<<(sf::Packet& packet, const ol::Input& input)
	{
        packet << static_cast<uint8_t>(input.inputType) << static_cast<uint8_t>(input.eventType);
        packet << input.mouse.x << input.mouse.y << input.mouse.scroll;
        packet << input.keyboard.key;

        return packet;
	}

	sf::Packet& operator>>(sf::Packet& packet, ol::Input& input)
    {
        // These really need to be a custom defined type for consistency.
		uint8_t inputType;
		packet >> inputType;
		input.inputType = static_cast<ol::eInputType>(inputType);

        uint8_t eventType;
        packet >> eventType;
        input.eventType = static_cast<ol::eEventType>(eventType);

        packet >> input.mouse.x >> input.mouse.y >> input.mouse.scroll;
        packet >> input.keyboard.key;

        return packet;
	}
}
