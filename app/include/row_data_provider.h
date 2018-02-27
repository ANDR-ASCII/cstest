#pragma once

namespace Test
{

struct RowData;

class RowDataProvider : public QObject
{
	Q_OBJECT

public:
	RowDataProvider(QObject* parent = nullptr);
	~RowDataProvider();

	void generateRowsData(std::size_t count);

signals:
	void rowDataReady(const RowData& rowData);

protected:
	virtual void timerEvent(QTimerEvent* event) override;

private:
	int m_dispatchTimerId;

	std::vector<std::future<RowData>> m_futures;
};

}