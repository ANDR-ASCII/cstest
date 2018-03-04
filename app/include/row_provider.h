#pragma once

#include "row_generator.h"

namespace Test
{
	
class RowDataExtractor;

class RowProvider : public QObject
{
	Q_OBJECT

public:
	RowProvider(QObject* parent = nullptr);

	void generateRows(std::size_t count);
	void stopGenerating();
	bool isGeneratingProcess() const noexcept;

signals:
	void rowsPackReady(const QVector<RowData>& row);
	void generatingDone();

protected:
	virtual void timerEvent(QTimerEvent* event) override;

private slots:
	void checkRowsReady();

private:
	std::unique_ptr<RowGenerator> m_rowGenerator;
	std::size_t m_generatedRowsCounter;
	int m_dispatchTimerId;
	bool m_isGeneratingProcess;
};

}