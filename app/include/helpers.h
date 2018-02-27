#pragma once

namespace Test
{

class Helpers final
{
public:
	using GeneratedValueType = std::mt19937::result_type;

	static GeneratedValueType randomValueFromRange(int from, int to);
};

}