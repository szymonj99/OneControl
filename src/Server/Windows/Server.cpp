#ifdef OS_WINDOWS

#include <OneControl/Server.h>

void oc::Server::ServerLoop()
{
	const auto processMouse = [&]
	{
		const auto mouseInterface = std::make_unique<ol::MouseSender>();
		// This will also act as a keep alive "feature".
		const auto timer = std::make_unique<ol::MessageTimer>(ol::kTimerTimeout, ol::eThreadMessages::Mouse, GetCurrentThreadId());
		while (ol::MouseSender::SendToClient)
		{
			oc::Packet pkt;
			const auto kMouseInput = mouseInterface->GetHookData();

			if (kMouseInput.type == ol::eInputType::Failed || kMouseInput.type == ol::eInputType::HookStopped || kMouseInput.type == ol::eInputType::Uninitialised)
			{
				ol::MouseSender::SendToClient = false;
				return;
			}

			pkt << kMouseInput;
			
			if (!SendPacketToClient(pkt))
			{
				ol::MouseSender::SendToClient = false;
			}
		}
	};
	std::thread mouseThread(processMouse);

	const auto processKeyboard = [&]
	{
		const auto keyboardInterface = std::make_unique<ol::KeyboardSender>(true);
		// This will also act as a keep alive "feature".
		const auto timer = std::make_unique<ol::MessageTimer>(ol::kTimerTimeout, ol::eThreadMessages::Keyboard, GetCurrentThreadId());
		while (ol::KeyboardSender::SendToClient)
		{
			oc::Packet pkt;
			const auto kKeyboardInput = keyboardInterface->GetHookData();

			if (kKeyboardInput.type == ol::eInputType::Failed || kKeyboardInput.type == ol::eInputType::HookStopped || kKeyboardInput.type == ol::eInputType::Uninitialised)
			{
				ol::KeyboardSender::SendToClient = false;
				return;
			}

			pkt << kKeyboardInput;
			
			if (!SendPacketToClient(pkt))
			{
				ol::KeyboardSender::SendToClient = false;
			}
		}
	};
    std::thread keyboardThread(processKeyboard);

	mouseThread.join();
	keyboardThread.join();
}

#endif
