#include "row_data_extractor.h"
#include "row_generator.h"

namespace
{

class InterruptionException : std::exception {};

constexpr std::size_t s_coefficient = 1000;

}

namespace Test
{

RowDataExtractor::RowDataExtractor(RowGenerator* rowGenerator, std::size_t extractCount, QObject* parent)
	: QObject(parent)
	, m_rowGenerator(rowGenerator)
	, m_extractCount(extractCount)
	, m_interruptOperation(false)
{
	qRegisterMetaType<RowData>("RowData");
}

void RowDataExtractor::interrupt() noexcept
{
	m_interruptOperation.store(true);
}

void RowDataExtractor::startExtraction()
{
	m_interruptOperation.store(false);

	std::size_t extractedCount = 0;

	const std::size_t rowsInPacket = qMax(std::ceil(m_extractCount / s_coefficient), 1.0);

	while (extractedCount < m_extractCount)
	{
		bool isInterrupted = false;

		QVector<RowData> rows;
		rows.reserve(static_cast<int>(rowsInPacket));

		const std::size_t minVal = qMin(rowsInPacket, m_extractCount - extractedCount);

		while(rows.size() < minVal)
		{
			RowData row;

			if (m_rowGenerator->extractRowData(row))
			{
				rows.push_back(row);
				++extractedCount;
			}

			try
			{
				interruptionPoint();
			}
			catch (const InterruptionException&)
			{
				isInterrupted = true;
				break;
			}

			std::this_thread::yield();
		}

		if (isInterrupted)
		{
			break;
		}

		emit rowsPackReady(rows);

		std::this_thread::yield();
	}

	emit done();
}

void RowDataExtractor::interruptionPoint() const
{
	if (!m_interruptOperation)
	{
		return;
	}

	throw InterruptionException();
}

}