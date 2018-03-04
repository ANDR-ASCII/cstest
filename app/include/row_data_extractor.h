#pragma once

namespace Test
{

class RowGenerator;
struct RowData;

class RowDataExtractor : public QObject
{
	Q_OBJECT

public:
	RowDataExtractor(RowGenerator* rowGenerator, std::size_t extractCount, QObject* parent = nullptr);

	void interrupt() noexcept;

signals:
	void rowsPackReady(const QVector<RowData>& row);
	void done();

private slots:
	void startExtraction();

private:
	void interruptionPoint() const;

private:
	RowGenerator* m_rowGenerator;
	std::size_t m_extractCount;
	std::atomic_bool m_interruptOperation;
};

}