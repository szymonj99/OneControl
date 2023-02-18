#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>

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
    std::ostream& operator<<(std::ostream& stream, const ol::eEventType& kEventType);

    std::istream& operator>>(std::istream& stream, ol::MouseInput& mouseInput);
    std::ostream& operator<<(std::ostream& stream, const ol::MouseInput& kMouseInput);

    std::istream& operator>>(std::istream& stream, ol::KeyboardInput& keyboardInput);
    std::ostream& operator<<(std::ostream& stream, const ol::KeyboardInput& kKeyboardInput);

    std::istream& operator>>(std::istream& stream, ol::Input& input);
    std::ostream& operator<<(std::ostream& stream, const ol::Input& kInput);

    // TODO: Figure out if we need to do:
    // std::istringstream and std::ostringstream, or if we can just use std::stringstream
    // For now, I will add both.

    std::stringstream& operator>>(std::stringstream& stream, ol::eInputType& inputType);
    std::istringstream& operator>>(std::istringstream& stream, ol::eInputType& inputType);

    std::stringstream& operator<<(std::stringstream& stream, const ol::eInputType& kInputType);
    std::ostringstream& operator<<(std::ostringstream& stream, const ol::eInputType& kInputType);
    
    std::stringstream& operator>>(std::stringstream& stream, ol::eEventType& eventType);
    std::istringstream& operator>>(std::istringstream& stream, ol::eEventType& eventType);

    std::stringstream& operator<<(std::stringstream& stream, const ol::eEventType& kEventType);
    std::ostringstream& operator<<(std::ostringstream& stream, const ol::eEventType& kEventType);
    
    std::stringstream& operator>>(std::stringstream& stream, ol::MouseInput& mouseInput);
    std::istringstream& operator>>(std::istringstream& stream, ol::MouseInput& mouseInput);

    std::stringstream& operator<<(std::stringstream& stream, const ol::MouseInput& kMouseInput);
    std::ostringstream& operator<<(std::ostringstream& stream, const ol::MouseInput& kMouseInput);
    
    std::stringstream& operator>>(std::stringstream& stream, ol::KeyboardInput& keyboardInput);
    std::istringstream& operator>>(std::istringstream& stream, ol::KeyboardInput& keyboardInput);

    std::stringstream& operator<<(std::stringstream& stream, const ol::KeyboardInput& kKeyboardInput);
    std::ostringstream& operator<<(std::ostringstream& stream, const ol::KeyboardInput& kKeyboardInput);
    
    std::stringstream& operator>>(std::stringstream& stream, ol::Input& input);
    std::istringstream& operator>>(std::istringstream& stream, ol::Input& input);

    std::stringstream& operator<<(std::stringstream& stream, const ol::Input& kInput);
    std::ostringstream& operator<<(std::ostringstream& stream, const ol::Input& kInput);

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
