#pragma once

namespace Test
{

class Helpers final
{
public:
	using GeneratedValueType = std::mt19937::result_type;

	template <typename DistributionType = std::uniform_int_distribution<>, typename ParamType = DistributionType::result_type>
	static GeneratedValueType randomValueFromRange(ParamType from, ParamType to)
	{
		static std::recursive_mutex generatorMutex;
		static std::mt19937 generator(std::random_device{}());

		std::lock_guard<std::recursive_mutex> locker(generatorMutex);
		DistributionType distribution(from, to);

		return distribution(generator);
	}

	static bool randomBoolValue();
};

}