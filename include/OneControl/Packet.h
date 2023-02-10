#pragma once

#include <iostream>

#include <SFML/Network/Packet.hpp>

namespace oc
{
    /**
     * The packet used to send the input data to a client.
     */
	class Packet : public sf::Packet
	{
	public:
        /**
         * This function is called very time a packet is received.
         * @param data Pointer to the packet contents
         * @param size The size of the data in the packet
         */
		void onReceive(const void* data, std::size_t size) override;
        /**
         * Called just before the packet is sent, meaning the data can be changed here.
         * @param size The size of the data in the packet
         * @return Pointer to the packet contents
         */
		const void* onSend(std::size_t& size) override;
	};
}
