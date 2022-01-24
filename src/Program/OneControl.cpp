#include "OneControl.h"

void oc::OneControl::Start()
{
	if (oc::customState)
	{
		m_eState = oc::parsedState;
	}
	else
	{
		m_eState = GetMachineState();
	}	
	ClearConsole();
	m_StartService();
}

void oc::OneControl::m_StartService()
{
	if (m_eState == eMachineState::Server)
	{
		pServer = move(std::make_unique<oc::Server>());
		std::thread serverThread([&] {
			pServer->Start();
			});

		serverThread.join();
	}
	else if (m_eState == eMachineState::Client)
	{
		pClient = move(std::make_unique<oc::Client>());
		std::thread clientThread([&] {
			pClient->Start();
			});

		clientThread.join();
	}
}
