#ifdef _WIN32

#include "../Server.h"

void oc::Server::ServerLoop()
{
	const auto processMouse = [&]
	{
		const auto mouseInterface = std::make_unique<oc::MouseSender>();
		mouseInterface->StartHook();
		const auto timer = std::make_unique<oc::MessageTimer>(1000, oc::eThreadMessages::Mouse, GetCurrentThreadId());
		while (oc::MouseSender::SendToClient)
		{
			sf::Packet pkt;
			const auto kMousePair = mouseInterface->GetHookData();

			if (kMousePair != oc::MousePair(INT32_MIN, INT32_MIN))
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::Mouse) << kMousePair.first << kMousePair.second;
				SendPacketToClient(pkt);
			}
		}
		mouseInterface->EndHook();
		timer->~MessageTimer();
	};
	std::thread mouseThread(processMouse);

	const auto processKeyboard = [&]
	{
		const auto keyboardInterface = std::make_unique<oc::KeyboardSender>();
		keyboardInterface->StartHook();
		const auto timer = std::make_unique<oc::MessageTimer>(1000, oc::eThreadMessages::Keyboard, GetCurrentThreadId());
		while (oc::KeyboardSender::SendToClient)
		{
			sf::Packet pkt;
			const auto kKeyboardPair = keyboardInterface->GetHookData();

			if (kKeyboardPair != oc::KeyboardPair(INT32_MIN, INT32_MIN))
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::Keyboard) << kKeyboardPair.first << kKeyboardPair.second;
				SendPacketToClient(pkt);
			}
		}
		keyboardInterface->EndHook();
		timer->~MessageTimer();
	};
    std::thread keyboardThread(processKeyboard);

	mouseThread.join();
	keyboardThread.join();
}

#endif
