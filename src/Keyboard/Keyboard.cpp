#include "Keyboard.h"

// Ideally this would take in a smart pointer instead.
void oc::Keyboard::SetServer(oc::Server* server)
{
	m_pServer = server;
}