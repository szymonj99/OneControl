#include <OneControl/Input.h>

// We need to add more options to this in the future.
namespace oc
{
    sf::Packet& operator<<(sf::Packet& packet, const ol::eInputType& kInputType)
    {
        packet << static_cast<uint8_t>(kInputType);
        return packet;
    }

    sf::Packet& operator>>(sf::Packet& packet, ol::eInputType& inputType)
    {
        uint8_t i;
        packet >> i;
        inputType = static_cast<ol::eInputType>(i);
        return packet;
    }

    sf::Packet& operator<<(sf::Packet& packet, const ol::eEventType& kEventType)
    {
        packet << static_cast<uint8_t>(kEventType);
        return packet;
    }

    sf::Packet& operator>>(sf::Packet& packet, ol::eEventType& eventType)
    {
        uint8_t i;
        packet >> i;
        eventType = static_cast<ol::eEventType>(i);
        return packet;
    }

    sf::Packet& operator<<(sf::Packet& packet, const ol::MouseInput& kMouseInput)
    {
        packet << kMouseInput.x << kMouseInput.y << kMouseInput.scroll;
        return packet;
    }

    sf::Packet& operator>>(sf::Packet& packet, ol::MouseInput& mouseInput)
    {
        packet >> mouseInput.x >> mouseInput.y >> mouseInput.scroll;
        return packet;
    }

    sf::Packet& operator<<(sf::Packet& packet, const ol::KeyboardInput& kKeyboardInput)
    {
        packet << kKeyboardInput.key;
        return packet;
    }

    sf::Packet& operator>>(sf::Packet& packet, ol::KeyboardInput& keyboardInput)
    {
        packet >> keyboardInput.key;
        return packet;
    }

	// Let's maybe serialize this in the future.
	sf::Packet& operator<<(sf::Packet& packet, const ol::Input& kInput)
	{
        packet << kInput.inputType << kInput.eventType << kInput.mouse << kInput.keyboard;
        return packet;
	}

	sf::Packet& operator>>(sf::Packet& packet, ol::Input& input)
    {
        packet >> input.inputType >> input.eventType >> input.mouse >> input.keyboard;
        return packet;
	}
}
