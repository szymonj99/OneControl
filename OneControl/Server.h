#pragma once

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <SFML/Network.hpp>

#include "Constants.h"

namespace oc
{
	class ocServer
	{
	private:
		std::unique_ptr<sf::TcpSocket> m_pClient = nullptr;
		std::unique_ptr<sf::TcpListener> m_pListener = nullptr;

	public:
		void Create();
		void Start();
		void SetClient(std::unique_ptr<sf::TcpSocket>& client);
		std::unique_ptr<sf::TcpSocket>& GetClient();
	};
}
