#pragma once

#include <memory>
#include <SFML/Network.hpp>

namespace oc
{
	class ocClient
	{
	private:
		std::unique_ptr<sf::TcpSocket> m_pServer;

	public:
		void Start();
	};
}
