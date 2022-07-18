#ifdef OS_WINDOWS

#include <OneControl/Server.h>

void oc::Server::ServerLoop()
{
	const auto processMouse = [&]
	{
		const auto mouseInterface = std::make_unique<oc::MouseSender>();
		// This will also act as a keep alive "feature".
		const auto timer = std::make_unique<oc::MessageTimer>(kTimerTimeout, oc::eThreadMessages::Mouse, GetCurrentThreadId());
		while (oc::MouseSender::SendToClient)
		{
			oc::Packet pkt;
			const auto kMouseInput = mouseInterface->GetHookData();

			if (kMouseInput.type == oc::eInputType::Failed || kMouseInput.type == oc::eInputType::HookStopped || kMouseInput.type == oc::eInputType::Uninitialised)
			{
				oc::MouseSender::SendToClient = false;
				return;
			}

			pkt << kMouseInput;
			
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
		const auto timer = std::make_unique<oc::MessageTimer>(kTimerTimeout, oc::eThreadMessages::Keyboard, GetCurrentThreadId());
		while (oc::KeyboardSender::SendToClient)
		{
			oc::Packet pkt;
			const auto kKeyboardInput = keyboardInterface->GetHookData();

			if (kKeyboardInput.type == oc::eInputType::Failed || kKeyboardInput.type == oc::eInputType::HookStopped || kKeyboardInput.type == oc::eInputType::Uninitialised)
			{
				oc::KeyboardSender::SendToClient = false;
				return;
			}

			pkt << kKeyboardInput;
			
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
