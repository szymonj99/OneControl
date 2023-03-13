#include <OneControl/OneControl.h>

void oc::OneControl::Start()
{
    std::cout << "You can also run OneControl with arguments. Run 'OneControl -h' to see the possible options." << std::endl;
	this->m_eState = oc::GetMachineState();
	this->m_fStartService();
}

void oc::OneControl::m_fStartService()
{
	if (this->m_eState == eMachineState::Server)
	{
		std::make_unique<oc::Server>()->Start();
	}
	else if (this->m_eState == eMachineState::Client)
	{
		std::make_unique<oc::Client>()->Start();
	}
}
