#include "helpers.h"

namespace Test
{

Helpers::GeneratedValueType Helpers::randomValueFromRange(int from, int to)
{
	static std::recursive_mutex generatorMutex;
	static std::mt19937 generator(std::random_device{}());

	std::lock_guard<std::recursive_mutex> locker(generatorMutex);
	std::uniform_int_distribution<> distribution(from, to);

	return distribution(generator);
}

}