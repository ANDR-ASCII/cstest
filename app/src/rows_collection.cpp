#include "rows_collection.h"
#include "row_generator.h"

namespace
{

template <typename T>
struct StdArraySize;

template <typename T, int N>
struct StdArraySize<std::array<T, N>>
{
	static constexpr int value = N;
};

class ReadLockerGuard final
{
public:
	ReadLockerGuard(QReadWriteLock& rwMutex)
		: m_rwMutex(rwMutex)
	{
		m_rwMutex.lockForRead();
	}

	~ReadLockerGuard()
	{
		m_rwMutex.unlock();
	}

private:
	QReadWriteLock & m_rwMutex;
};

class WriteLockerGuard final
{
public:
	WriteLockerGuard(QReadWriteLock& rwMutex)
		: m_rwMutex(rwMutex)
	{
		m_rwMutex.lockForWrite();
	}

	~WriteLockerGuard()
	{
		m_rwMutex.unlock();
	}

private:
	QReadWriteLock & m_rwMutex;
};

}

namespace Test
{

RowsCollection::RowsCollection(QObject* parent)
	: QObject(parent)
{
}

void RowsCollection::addRow(const RowData& rowData)
{
	RowDataType row;

	row[0] = QVariant::fromValue(QString(rowData.string));
	row[1] = QVariant::fromValue(rowData.number);
	row[2] = QVariant::fromValue(rowData.floatingPointNumber);
	row[3] = QVariant::fromValue(rowData.boolValue);

	WriteLockerGuard guard(m_rwMutex);
	m_rows.push_back(row);

	emit rowAdded();
}

void RowsCollection::clear()
{
	WriteLockerGuard guard(m_rwMutex);
	m_rows.clear();

	emit allRowsRemoved();
}

int RowsCollection::rowCount() const noexcept
{
	ReadLockerGuard guard(m_rwMutex);
	return m_rows.size();
}

int RowsCollection::columnCount() const noexcept
{
	return StdArraySize<RowDataType>::value;
}

QVariant RowsCollection::itemAt(int row, int column)
{
	ASSERT(column < columnCount());
	ASSERT(row < rowCount());

	ReadLockerGuard guard(m_rwMutex);
	return m_rows[row][column];
}

}