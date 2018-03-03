#pragma once

namespace Test
{

struct RowData;

class BrokenDataResultOperation : public std::runtime_error
{
public:
	explicit BrokenDataResultOperation(const std::string& what_arg);
	explicit BrokenDataResultOperation(const char* what_arg);
};

class RowsCollection : public QObject
{
	Q_OBJECT

public:
	RowsCollection(QObject* parent = nullptr);

	void addRow(const RowData& rowData);
	void clear();

	int rowCount() const noexcept;
	int columnCount() const noexcept;

	QVariant itemAt(int row, int column) const;

	bool canWriteFromThisThread() const noexcept;
	void setSerializationState(bool value) noexcept;

signals:
	void rowAdded(int index);
	void allRowsRemoved();

private:
	using RowDataType = std::array<QVariant, 4>;

	class SerializationStateDescriptor
	{
	public:
		SerializationStateDescriptor();

		void setSerializationState(bool value) noexcept;
		bool canWriteFromThisThread() const noexcept;

	private:
		mutable std::mutex m_mutex;
		bool m_serializationState;
		std::thread::id m_id;
	};

	QVector<RowDataType> m_rows;
	mutable QReadWriteLock m_rwMutex;

	SerializationStateDescriptor m_serializationStateDescriptor;
};

}