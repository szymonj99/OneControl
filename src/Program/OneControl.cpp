﻿#include "OneControl.h"

void oc::OneControl::m_StartService()
{
	if (m_eState == eMachineState::Server)
	{
		std::make_unique<ocServer>()->Start();
	}
	else if (m_eState == eMachineState::Client)
	{
		std::make_unique<ocClient>()->Start();
	}
}

void oc::OneControl::Start()
{
	m_eState = GetMachineState();
	ClearConsole();
	m_StartService();
}
