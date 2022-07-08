#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <string_view>

#include <OneControl/Constants.h>
#include <OneControl/Enums.h>
#include <OneControl/HelperMethods.h>
#include <OneControl/Version.h>
#include <OneControl/Server.h>
#include <OneControl/Client.h>

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
