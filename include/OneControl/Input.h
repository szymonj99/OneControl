#pragma once

#include <cstdint>

#include <SFML/Network/Packet.hpp>

#include <OneLibrary/Input.h>

namespace oc
{
    /**
     * Append the input data to the packet.
     * TODO: Serialize this in the future, potentially using Google's `protobuf`
     * @param packet
     * @param input
     * @return The packet
     */
	sf::Packet& operator<<(sf::Packet& packet, const ol::Input& input);
    /**
     * Retrieve the input data from the packet.
     * TODO: Serialize this in the future, potentially using Google's `protobuf`
     * @param packet
     * @param input
     * @return The packet
     */
	sf::Packet& operator>>(sf::Packet& packet, ol::Input& input);
}
