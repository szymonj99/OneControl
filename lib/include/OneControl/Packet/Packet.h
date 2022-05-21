#pragma once

#include <iostream>

#include <SFML/Network/Packet.hpp>

namespace oc
{
	class Packet : public sf::Packet
	{
	public:
		virtual void onReceive(const void* data, std::size_t size);
		virtual const void* onSend(std::size_t& size);
	};
}
