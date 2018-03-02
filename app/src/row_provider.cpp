#include "row_provider.h"
#include "helpers.h"

namespace Test
{

RowProvider::RowProvider(QObject* parent)
	: QObject(parent)
	, m_rowGenerator(std::make_unique<RowGenerator>())
	, m_generatedRowsCounter(0)
	, m_dispatchTimerId(0)
	, m_isGeneratingProcess(false)
{
}

void RowProvider::generateRows(std::size_t count)
{
	ASSERT(count);

	m_generatedRowsCounter = count;

	m_dispatchTimerId = startTimer(Helpers::s_minimumRecommendedTimerResolution);
	ASSERT(m_dispatchTimerId);

	m_rowGenerator->generateRows(count);

	m_isGeneratingProcess = true;
}

void RowProvider::stopGenerating()
{
	if (m_dispatchTimerId)
	{
		m_rowGenerator->stopGenerating();

		killTimer(m_dispatchTimerId);

		m_isGeneratingProcess = false;

		emit generatingDone();
	}
}

void RowProvider::timerEvent(QTimerEvent* event)
{
	checkRowsReady();

	QObject::timerEvent(event);
}

void RowProvider::checkRowsReady()
{
	RowData row;

	if (m_rowGenerator->extractRowData(row))
	{
		emit rowReady(row);

		--m_generatedRowsCounter;
	}

	if (!m_generatedRowsCounter)
	{
		ASSERT(m_dispatchTimerId);
		killTimer(m_dispatchTimerId);

		m_isGeneratingProcess = false;

		emit generatingDone();
	}
}

bool RowProvider::isGeneratingProcess() const noexcept
{
	return m_isGeneratingProcess;
}

}