#include "row_generator.h"
#include "helpers.h"

namespace Test
{

void RowGenerator::generateRows(std::size_t count)
{
	const auto task = [this]
	{
		QList<QStandardItem*> row;

		QString string;
		QString number;
		QString floatingPointNumber;
		QString boolValue;

		constexpr int stringLength = 5;

		for (int i = 0; i < stringLength; ++i)
		{
			string += static_cast<char>(Helpers::randomValueFromRange('a', 'z'));
		}

		row.push_back(new QStandardItem(string));
		row.push_back(new QStandardItem(QString::fromUtf8("%1").arg(Helpers::randomValueFromRange(0, 100))));
		row.push_back(new QStandardItem(QString::fromUtf8("%1").arg(Helpers::randomValueFromRange<std::uniform_real_distribution<>>(0.0, 100.0))));
		row.push_back(new QStandardItem(Helpers::randomBoolValue() ? QString::fromUtf8("true") : QString::fromUtf8("false")));

		m_queue.push(std::move(row));
	};

	for (std::size_t i = 0; i < count; ++i)
	{
		m_threadPool.pushDetachedTask(task);
	}
}

bool RowGenerator::extractRowData(QList<QStandardItem*>& row)
{
	return m_queue.pop(row);
}

}