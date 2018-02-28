#include "row_provider.h"

namespace
{

constexpr int c_minimumRecommendedTimerResolution = 21;

}

namespace Test
{

RowProvider::RowProvider(QObject* parent)
	: QObject(parent)
	, m_rowGenerator(std::make_unique<RowGenerator>())
	, m_generatedRowsCounter(0)
	, m_dispatchTimerId(0)
{
}

void RowProvider::generateRows(std::size_t count)
{
	ASSERT(count);

	m_generatedRowsCounter = count;

	m_dispatchTimerId = startTimer(c_minimumRecommendedTimerResolution);
	ASSERT(m_dispatchTimerId);

	m_rowGenerator->generateRows(count);
}

void RowProvider::timerEvent(QTimerEvent* event)
{
	checkRowsReady();

	QObject::timerEvent(event);
}

void RowProvider::checkRowsReady()
{
	QList<QStandardItem*> row;

	if (m_rowGenerator->extractRowData(row))
	{
		emit rowReady(row);

		--m_generatedRowsCounter;
	}

	if (!m_generatedRowsCounter)
	{
		ASSERT(m_dispatchTimerId);
		killTimer(m_dispatchTimerId);
	}
}

}