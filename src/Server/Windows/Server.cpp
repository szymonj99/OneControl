#ifdef _WIN32

#include "../Server.h"

void oc::Server::ServerLoop()
{
	const auto processMouse = [](oc::Server* server)
	{
		auto mouseInterface = std::make_unique<oc::MouseSender>();
		mouseInterface->StartHook();
		while (true)
		{
			sf::Packet pkt;

			const auto kMousePair = mouseInterface->GetHookData();
			pkt << static_cast<oc::InputInt>(oc::eInputType::Mouse) << kMousePair.first << kMousePair.second;
			server->SendPacketToClient(pkt);
		}
		mouseInterface->EndHook();
	};	
	std::thread mouseThread(processMouse, this);

	const auto processKeyboard = [](oc::Server* server)
	{
		auto keyboardInterface = std::make_unique<oc::KeyboardSender>();
		keyboardInterface->StartHook();
		while (true)
		{
			sf::Packet pkt;

			const auto kKeyboardPair = keyboardInterface->GetHookData();
			pkt << static_cast<oc::InputInt>(oc::eInputType::Keyboard) << kKeyboardPair.first << kKeyboardPair.second;
			server->SendPacketToClient(pkt);
		}
		keyboardInterface->EndHook();
	};
	std::thread keyboardThread(processKeyboard, this);

	mouseThread.join();
	keyboardThread.join();
}

#endif
