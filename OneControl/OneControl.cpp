#include "OneControl.h"

void oc::OneControl::m_StartService()
{
	if (m_eState == eMachineState::Server)
	{
		auto server = std::make_unique<ocServer>();
		server->Start();
	}
	else if (m_eState == eMachineState::Client)
	{
		auto client = std::make_unique<ocClient>();
		client->Start();
	}
}

void oc::OneControl::Start()
{
	m_eState = GetMachineState();
	ClearConsole();
	m_StartService();
}
