#include "RSA.h"

namespace oc
{
	uint256_t GetCubanPrime(uint256_t x, uint256_t y)
	{
		if (x < y + 2)
		{
			std::cout << "Improper values. x was not bigger than y + 2\n";
			x = y + 2;
		}

		return ((x * x * x) - (y * y) / (x - y));
	}

	uint256_t GetRandom()
	{
		return static_cast<uint256_t>(rand() * rand() * sqrt(rand()));
	}
}