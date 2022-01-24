#include "MouseSender.h"

// Ideally this would take in a smart pointer instead.
void oc::MouseSender::SetServer(oc::Server* server)
{
	m_pServer = server;
}
