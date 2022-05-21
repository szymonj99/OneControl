#pragma once

#include <memory>
#include <iostream>
#include <utility>
#include <cstdint>
#include <thread>

#include <fmt/core.h>
#include <fmt/color.h>

#include <OneControl/Constants.h>
#include <OneControl/Enums.h>

namespace oc
{
	// This class is what will be sent from the Server to the Client.
	// I think for this, adding some tests would be easy.
	class Input
	{
	private:
		oc::MouseInt x = 0;
		oc::MouseInt y = 0;

		oc::KeyboardInt key = 0;
		oc::KeyboardInt state = 0;

	public:
		Input();
		~Input();
	};
}
