#include <OneControl/OneControl.h>

void oc::OneControl::Start()
{
    std::cout << "You can also run OneControl with arguments. Run 'OneControl -h' to see the possible options.\n";
	m_eState = GetMachineState();
	//ClearConsole();
	m_StartService();
}

void oc::OneControl::m_StartService()
{
    // TODO: Potentially move these into m_StartServer and m_StartClient.
	if (m_eState == eMachineState::Server)
	{
		pServer = std::move(std::make_unique<oc::Server>());
		std::thread serverThread([&] {
			pServer->Start();
			});

		serverThread.join();
	}
	else if (m_eState == eMachineState::Client)
	{
		pClient = std::move(std::make_unique<oc::Client>());
		std::thread clientThread([&] {
			pClient->Start();
			});

		clientThread.join();
	}
}
