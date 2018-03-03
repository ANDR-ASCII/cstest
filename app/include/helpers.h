#pragma once

namespace Test
{

class Helpers final
{
public:
	template <typename DistributionType = std::uniform_int_distribution<>, typename ParamType = DistributionType::result_type>
	static auto randomValueFromRange(ParamType from, ParamType to)
	{
		static std::recursive_mutex generatorMutex;
		static std::default_random_engine generator(std::random_device{}());

		std::lock_guard<std::recursive_mutex> locker(generatorMutex);
		DistributionType distribution(from, to);

		return distribution(generator);
	}

	template <typename T, std::size_t N>
	constexpr static std::size_t arrayLength(T(&arr)[N])
	{
		return N;
	}

	static bool randomBoolValue();

	static constexpr int s_minimumRecommendedTimerResolution = 21;
};

}