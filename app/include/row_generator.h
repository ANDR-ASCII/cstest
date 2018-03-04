#pragma once

#include "thread_safe_queue.h"
#include "thread_pool.h"

namespace Test
{

struct RowData
{
	char string[3];
	int number;
	double floatingPointNumber;
	bool boolValue;
};

Q_DECLARE_METATYPE(RowData)

class RowGenerator final
{
public:
	void generateRows(std::size_t count);
	bool extractRowData(RowData& row);
	bool extractAllRowsData(QVector<RowData>& rows);
	bool extractRowsDataMax(QVector<RowData>& rows, std::size_t max);
	void stopGenerating();
	std::size_t size() const noexcept;

private:
	//
	// Order of members is important!
	//
	ThreadSafeQueue<RowData> m_queue;
	ThreadPool m_threadPool;
};

}