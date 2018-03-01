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

class RowGenerator final
{
public:
	void generateRows(std::size_t count);
	bool extractRowData(RowData& row);
	void stopGenerating();

private:
	//
	// Order of members is important!
	//
	ThreadSafeQueue<RowData> m_queue;
	ThreadPool m_threadPool;
};

}