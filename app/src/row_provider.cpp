#include "row_provider.h"
#include "helpers.h"
#include "row_data_extractor.h"

namespace Test
{

RowProvider::RowProvider(QObject* parent)
	: QObject(parent)
	, m_rowGenerator(std::make_unique<RowGenerator>())
	, m_generatedRowsCounter(0)
	, m_dispatchTimerId(0)
	, m_isGeneratingProcess(false)
	, m_extractorThread(new QThread)
{
	m_extractorThread->start();
}

RowProvider::~RowProvider()
{
	stopGenerating();

	m_extractorThread->exit();
	m_extractorThread->wait();
	m_rowGenerator->wait();
}

void RowProvider::generateRows(std::size_t count)
{
	ASSERT(!m_isGeneratingProcess);

	m_extractor = new RowDataExtractor(m_rowGenerator.get(), count);
	m_extractor->moveToThread(m_extractorThread);

	VERIFY(connect(m_extractor, &RowDataExtractor::rowsPackReady, this, &RowProvider::rowsPackReady, Qt::QueuedConnection));
	VERIFY(connect(m_extractor, &RowDataExtractor::done, this, &RowProvider::onExtractionDone, Qt::QueuedConnection));

	m_rowGenerator->generateRows(count);

	m_isGeneratingProcess = true;

	VERIFY(QMetaObject::invokeMethod(m_extractor, "startExtraction", Qt::QueuedConnection));
}

void RowProvider::stopGenerating()
{
	if (!m_extractor)
	{
		return;
	}

	m_rowGenerator->stopGenerating();
	m_extractor->interrupt();
	onExtractionDone();
}

void RowProvider::onExtractionDone()
{
	m_extractor->deleteLater();

	m_isGeneratingProcess = false;

	emit generatingDone();
}

bool RowProvider::isGeneratingProcess() const noexcept
{
	return m_isGeneratingProcess;
}

}