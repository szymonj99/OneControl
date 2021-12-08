#include "Mouse.h"

// Ideally this would take in a smart pointer instead.
void oc::Mouse::SetServer(oc::Server* server)
{
	m_pServer = server;
}
