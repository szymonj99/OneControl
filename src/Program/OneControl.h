#pragma once

#define UNICODE 1

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <string_view>
#include <SFML/Network.hpp>
//#include <botan/botan.h>
//#include <cryptopp/rsa.h>
//#include <openssl/rsa.h>

#include "../Helpers/Constants.h"
#include "../Helpers/Enums.h"
#include "../Helpers/StaticHelpers.h"
#include "../Version/Version.h"
#include "../Server/Server.h"
#include "../Client/Client.h"

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
