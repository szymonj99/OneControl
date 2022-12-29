#ifdef OS_APPLE

#include <OneControl/Server.h>

void oc::Server::ServerLoop()
{
	const auto temporaryLambda = [](oc::Server* server)
	{
		while (true)
		{
			oc::Packet pkt;
			pkt << static_cast<oc::InputInt>(oc::eInputType::Mouse);
			oc::MousePair pair{ 0,0 };
			pkt << pair.first << pair.second;
			server->SendPacketToClient(pkt);
			std::this_thread::sleep_for(std::chrono::seconds{ 1 });
		}
	};
	std::thread temporaryThread(temporaryLambda, this);
	temporaryThread.join();
}

#endif
