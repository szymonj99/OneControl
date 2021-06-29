#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <string_view>
#include <SFML/Network.hpp>
//#include <botan/botan.h>
//#include <cryptopp/rsa.h>
//#include <openssl/rsa.h>

#include "Constants.h"
#include "Enums.h"
#include "Version.h"
#include "StaticHelpers.h"
#include "Server.h"
#include "Client.h"

namespace oc
{
	class OneControl
	{
	private:
		eMachineState m_eState;
		void m_StartService();

	public:
		void Start();
	};
}
