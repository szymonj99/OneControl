#pragma once

#include <cstdint>
#include <iostream>

#include <SFML/Network/Packet.hpp>

#include <OneLibrary/Input.h>

// This is used to have a slightly better syntax when sending across the ol::Input things.
// I think this could have been implemented in OneLibrary directly with input and output streams, but ah well.
namespace oc
{
    // TODO: Move these stream operators to OneLibrary

    std::istream& operator>>(std::istream& stream, ol::eInputType& inputType);
    std::ostream& operator<<(std::ostream& stream, const ol::eInputType& kInputType);

    std::istream& operator>>(std::istream& stream, ol::eEventType& eventType);
    std::ostream& operator<<(std::ostream& stream, const ol::eInputType& kEventType);

    std::istream& operator>>(std::istream& stream, ol::MouseInput& mouseInput);
    std::ostream& operator<<(std::ostream& stream, const ol::MouseInput& kMouseInput);

    std::istream& operator>>(std::istream& stream, ol::KeyboardInput& keyboardInput);
    std::ostream& operator<<(std::ostream& stream, const ol::KeyboardInput& kKeyboardInput);

    std::istream& operator>>(std::istream& stream, ol::Input& input);
    std::ostream& operator<<(std::ostream& stream, const ol::Input& kInput);

    sf::Packet& operator<<(sf::Packet& packet, const ol::eInputType& kInputType);

    sf::Packet& operator>>(sf::Packet& packet, ol::eInputType& inputType);

    sf::Packet& operator<<(sf::Packet& packet, const ol::eEventType& kEventType);

    sf::Packet& operator>>(sf::Packet& packet, ol::eEventType& eventType);

    sf::Packet& operator<<(sf::Packet& packet, const ol::MouseInput& kMouseInput);

    sf::Packet& operator>>(sf::Packet& packet, ol::MouseInput& mouseInput);

    sf::Packet& operator<<(sf::Packet& packet, const ol::KeyboardInput& kKeyboardInput);

    sf::Packet& operator>>(sf::Packet& packet, ol::KeyboardInput& keyboardInput);

    /**
     * Append the input data to the packet.
     * TODO: Serialize this in the future, potentially using Google's `protobuf`
     * @param packet
     * @param input
     * @return The packet
     */
	sf::Packet& operator<<(sf::Packet& packet, const ol::Input& kInput);
    /**
     * Retrieve the input data from the packet.
     * TODO: Serialize this in the future, potentially using Google's `protobuf`
     * @param packet
     * @param input
     * @return The packet
     */
	sf::Packet& operator>>(sf::Packet& packet, ol::Input& input);
}
