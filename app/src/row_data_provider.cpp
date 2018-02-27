#include "row_data_provider.h"
#include "generate_row_task.h"

namespace
{

constexpr int s_minimumRecommendedTimerResolution = 21;

}

namespace Test
{

RowDataProvider::RowDataProvider(QObject* parent)
	: QObject(parent)
	, m_dispatchTimerId(0)
{
	qRegisterMetaType<RowData>("RowData");

	m_dispatchTimerId = startTimer(s_minimumRecommendedTimerResolution);

	ASSERT(m_dispatchTimerId);
}

RowDataProvider::~RowDataProvider()
{
	ASSERT(m_dispatchTimerId);
	killTimer(m_dispatchTimerId);
}

void RowDataProvider::generateRowsData(std::size_t count)
{
	for (std::size_t i = 0; i < count; ++i)
	{
		m_futures.push_back(std::async(std::launch::async, GenerateRowTask()));
	}
}

void RowDataProvider::timerEvent(QTimerEvent* event)
{
	const auto deleterWaiter = [this](std::future<RowData>& future)
	{
		if (future.wait_until(std::chrono::system_clock::now()) == std::future_status::ready)
		{
			emit rowDataReady(future.get());

			return true;
		}

		return false;
	};

	m_futures.erase(std::remove_if(m_futures.begin(), m_futures.end(), deleterWaiter), m_futures.end());

	QObject::timerEvent(event);
}

}