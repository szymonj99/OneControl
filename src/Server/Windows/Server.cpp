#ifdef _WIN32

#include "../Server.h"

void oc::Server::ServerLoop()
{
	const auto processMouse = [](oc::Server* server)
	{
		auto mouseInterface = std::make_unique<oc::MouseSender>();
		mouseInterface->StartHook();
		auto timer = SetTimer(0, 0, 1000, 0);
		while (true)
		{
			sf::Packet pkt;
			
			const auto kMousePair = mouseInterface->GetHookData();
			if (kMousePair != oc::MousePair(INT32_MIN, INT32_MIN))
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::Mouse) << kMousePair.first << kMousePair.second;
				server->SendPacketToClient(pkt);
			}
		}
		mouseInterface->EndHook();
		KillTimer(0, timer);
	};
	std::thread mouseThread(processMouse, this);

	const auto processKeyboard = [](oc::Server* server)
	{
		auto keyboardInterface = std::make_unique<oc::KeyboardSender>();
		keyboardInterface->StartHook();
		auto timer = SetTimer(0, 0, 1000, 0);
		while (true)
		{
			sf::Packet pkt;

			const auto kKeyboardPair = keyboardInterface->GetHookData();
			if (kKeyboardPair != oc::KeyboardPair(INT32_MIN, INT32_MIN))
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::Keyboard) << kKeyboardPair.first << kKeyboardPair.second;
				server->SendPacketToClient(pkt);
			}
		}
		keyboardInterface->EndHook();
		KillTimer(0, timer);
	};
	std::thread keyboardThread(processKeyboard, this);

	mouseThread.join();
	keyboardThread.join();
}

#endif
