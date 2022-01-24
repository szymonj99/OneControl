#include "KeyboardSender.h"

// Ideally this would take in a smart pointer instead.
void oc::KeyboardSender::SetServer(oc::Server* server)
{
	m_pServer = server;
}