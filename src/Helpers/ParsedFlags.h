#pragma once

#include "Enums.h"

namespace oc
{
	static uint16_t parsedPort = 0;
	static bool customPort = false;

	static oc::eMachineState parsedState = oc::eMachineState::Client;
	static bool customState = false;

	static sf::IpAddress parsedAddress = sf::IpAddress::LocalHost;
	static bool customAddress = false;
}