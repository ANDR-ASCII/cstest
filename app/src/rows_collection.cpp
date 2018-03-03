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
	WriteLockerGuard(QReadWriteLock& rwMutex, bool canWrite)
		: m_rwMutex(rwMutex)
	{
		if (!canWrite)
		{
			throw Test::BrokenDataResultOperation("Attempt to change collection at serialization/deserialization state");
		}

		m_rwMutex.lockForWrite();
	}

	~WriteLockerGuard()
	{
		m_rwMutex.unlock();
	}

private:
	QReadWriteLock& m_rwMutex;
};

}

namespace Test
{

BrokenDataResultOperation::BrokenDataResultOperation(const char* what_arg)
	: std::runtime_error(what_arg)
{
}

BrokenDataResultOperation::BrokenDataResultOperation(const std::string& what_arg)
	: std::runtime_error(what_arg)
{
}

//////////////////////////////////////////////////////////////////////////

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

	WriteLockerGuard guard(m_rwMutex, canWriteFromThisThread());
	m_rows.push_back(row);

	emit rowAdded();
}

void RowsCollection::clear()
{
	WriteLockerGuard guard(m_rwMutex, canWriteFromThisThread());
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

void RowsCollection::setSerializationState(bool value) noexcept
{
	m_serializationStateDescriptor.setSerializationState(value);
}

bool RowsCollection::canWriteFromThisThread() const noexcept
{
	return m_serializationStateDescriptor.canWriteFromThisThread();
}

//////////////////////////////////////////////////////////////////////////

RowsCollection::SerializationStateDescriptor::SerializationStateDescriptor()
	: m_serializationState(false)
	, m_id(std::thread::id())
{
}

void RowsCollection::SerializationStateDescriptor::setSerializationState(bool value) noexcept
{
	std::lock_guard<std::mutex> locker(m_mutex);

	m_serializationState = value;
	m_id = std::this_thread::get_id();
}

bool RowsCollection::SerializationStateDescriptor::canWriteFromThisThread() const noexcept
{
	std::lock_guard<std::mutex> locker(m_mutex);

	return m_serializationState ? m_id == std::this_thread::get_id() : true;
}

}