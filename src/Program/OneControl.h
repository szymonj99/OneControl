#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <string_view>

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
		eMachineState m_eState = eMachineState::Uninitialised;
		void m_StartService();

	public:
		void Start();
		std::unique_ptr<oc::Client> pClient = nullptr;
		std::unique_ptr<oc::Server> pServer = nullptr;
	};
}
