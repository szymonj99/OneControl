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
    /**
     * This class is used when running the program.
     */
	class OneControl
	{
	private:
		eMachineState m_eState = eMachineState::Uninitialised;
		void m_fStartService();

	public:
		/**
		 * Start the program. If the state/type of the current machine was passed in as a command-line argument, it will be used here. Otherwise, prompt the user.
		 */
		void Start();
	};
}
