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

	const auto processKeyboard = [](oc::Server* server)
	{
		auto keyboardInterface = std::make_unique<Keyboard>();
		keyboardInterface->SetServer(server);
		keyboardInterface->StartHook();
		keyboardInterface->EndHook();
	};
	std::thread keyboardThread(processKeyboard, this);

	mouseThread.join();
	keyboardThread.join();
}

#endif
