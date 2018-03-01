#include "row_generator.h"
#include "helpers.h"

namespace Test
{

void RowGenerator::generateRows(std::size_t count)
{
	const auto task = [this]
	{
		RowData row;

		for (int i = 0; i < Helpers::arrayLength(row.string) - 1; ++i)
		{
			row.string[i] = static_cast<char>(Helpers::randomValueFromRange('a', 'z'));
		}

		row.string[Helpers::arrayLength(row.string) - 1] = 0;

		row.number = Helpers::randomValueFromRange(0, 100);
		row.floatingPointNumber = Helpers::randomValueFromRange<std::uniform_real_distribution<>>(0.0, 100.0);
		row.boolValue = Helpers::randomBoolValue();

		m_queue.push(std::move(row));
	};

	const auto distributeTasks = [&]
	{
		for (std::size_t i = 0; i < count; ++i)
		{
			m_threadPool.pushDetachedTask(task);
		}
	};

	std::async(std::launch::async, distributeTasks);
}

bool RowGenerator::extractRowData(RowData& row)
{
	return m_queue.pop(row);
}

void RowGenerator::stopGenerating()
{
	m_threadPool.clearTasks();

	m_queue.clear();
}

}