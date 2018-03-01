#include "helpers.h"

namespace Test
{

bool Helpers::randomBoolValue()
{
	static std::recursive_mutex generatorMutex;
	static std::default_random_engine generator(std::random_device{}());

	std::lock_guard<std::recursive_mutex> locker(generatorMutex);
	std::bernoulli_distribution distribution;

	return distribution(generator);
}

}