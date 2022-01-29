#ifdef _WIN32

#include "../Server.h"

void oc::Server::ServerLoop()
{
	const auto processMouse = [&]
	{
		const auto mouseInterface = std::make_unique<oc::MouseSender>();
		// This will also act as a keep alive "feature".
		const auto timer = std::make_unique<oc::MessageTimer>(3000, oc::eThreadMessages::Mouse, GetCurrentThreadId());
		oc::MousePair previous = {0,0};
		while (oc::MouseSender::SendToClient)
		{
			sf::Packet pkt;
			const auto kMousePair = mouseInterface->GetHookData();

			if (kMousePair == oc::MousePair(INT32_MIN, INT32_MIN))
			{
				oc::MouseSender::SendToClient = false;
				return;
			}

			if (kMousePair == oc::MousePair(INT32_MAX, INT32_MAX))
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::KeepAlive);
			}
			else
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::Mouse) << kMousePair.first << kMousePair.second;
			}
			
			if (!SendPacketToClient(pkt))
			{
				oc::MouseSender::SendToClient = false;
			}
		}
	};
	std::thread mouseThread(processMouse);

	const auto processKeyboard = [&]
	{
		const auto keyboardInterface = std::make_unique<oc::KeyboardSender>();
		// This will also act as a keep alive "feature".
		const auto timer = std::make_unique<oc::MessageTimer>(3000, oc::eThreadMessages::Keyboard, GetCurrentThreadId());
		while (oc::KeyboardSender::SendToClient)
		{
			sf::Packet pkt;
			const auto kKeyboardPair = keyboardInterface->GetHookData();

			if (kKeyboardPair == oc::KeyboardPair(INT32_MIN, INT32_MIN))
			{
				oc::KeyboardSender::SendToClient = false;
				return;
			}

			if (kKeyboardPair == oc::KeyboardPair(INT32_MAX, INT32_MAX))
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::KeepAlive);
			}
			else
			{
				pkt << static_cast<oc::InputInt>(oc::eInputType::Keyboard) << kKeyboardPair.first << kKeyboardPair.second;
			}
			
			if (!SendPacketToClient(pkt))
			{
				oc::KeyboardSender::SendToClient = false;
			}
		}
	};
    std::thread keyboardThread(processKeyboard);

	mouseThread.join();
	keyboardThread.join();
}

#endif
