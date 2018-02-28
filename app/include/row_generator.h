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
	//
	// Order of members is important!
	//
	ThreadSafeQueue<QList<QStandardItem*>> m_queue;
	ThreadPool m_threadPool;
};

}