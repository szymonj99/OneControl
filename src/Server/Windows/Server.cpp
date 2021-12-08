#ifdef _WIN32

#include "../Server.h"

void oc::Server::ServerLoop()
{
	const auto processMouse = [](oc::Server* server)
	{
		auto mouseInterface = std::make_unique<Mouse>();
		mouseInterface->SetServer(server);
		mouseInterface->StartHook();
		mouseInterface->EndHook();
	};	
	std::thread mouseThread(processMouse, this);
	mouseThread.join();
}

#endif
