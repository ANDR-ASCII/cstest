#pragma once

namespace Test
{

struct RowData;

class RowDataProvider : public QObject
{
	Q_OBJECT

public:
	RowDataProvider(QObject* parent = nullptr);

	void generateRowsData() const;

signals:
	void rowDataReady(const RowData& rowData);
};

}