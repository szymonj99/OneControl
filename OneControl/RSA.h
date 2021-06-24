#pragma once

#include <cstdint>
#include <cmath>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>

namespace oc
{
	using uint256_t = boost::multiprecision::uint256_t;
	uint256_t GetCubanPrime(uint256_t x, uint256_t y);
	uint256_t GetRandom();
}
