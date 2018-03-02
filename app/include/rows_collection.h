#pragma once

namespace Test
{

struct RowData;

class RowsCollection : public QObject
{
	Q_OBJECT

public:
	RowsCollection(QObject* parent = nullptr);

	void addRow(const RowData& rowData);
	void clear();

	int rowCount() const noexcept;
	int columnCount() const noexcept;

	QVariant itemAt(int row, int column);

signals:
	void rowAdded();
	void allRowsRemoved();

private:
	using RowDataType = std::array<QVariant, 4>;

	QVector<RowDataType> m_rows;
	mutable QReadWriteLock m_rwMutex;
};

}