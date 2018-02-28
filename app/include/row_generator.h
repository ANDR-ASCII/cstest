#pragma once

#include "thread_safe_queue.h"
#include "thread_pool.h"

namespace Test
{

class RowGenerator final
{
public:
	void generateRows(std::size_t count);
	bool extractRowData(QList<QStandardItem*>& row);

private:
	ThreadPool m_threadPool;
	ThreadSafeQueue<QList<QStandardItem*>> m_queue;
};

}