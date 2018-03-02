#include "main_window.h"
#include "row_provider.h"
#include "tasks.h"
#include "helpers.h"
#include "rows_collection.h"
#include "custom_table_model.h"

namespace Test
{

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_rowProvider(new RowProvider(this))
	, m_rowsCollection(new RowsCollection(this))
	, m_saveLoadFileTimer(new QTimer(this))
{
	setupUi(this);

	VERIFY(connect(generateRowsButton, &QPushButton::clicked, this, &MainWindow::onGenerateRowsButtonClicked));
	VERIFY(connect(saveToFileButton, &QPushButton::clicked, this, &MainWindow::onSaveToFileButtonClicked));
	VERIFY(connect(loadFromFileButton, &QPushButton::clicked, this, &MainWindow::onLoadFromFileButtonClicked));
	VERIFY(connect(m_rowProvider, &RowProvider::rowReady, this, &MainWindow::onRowDataReady));
	VERIFY(connect(m_rowProvider, &RowProvider::generatingDone, this, &MainWindow::onGeneratingDone));
	VERIFY(connect(m_saveLoadFileTimer, &QTimer::timeout, this, &MainWindow::checkSaveLoadOperationReady));

	tableView->setModel(new CustomTableModel(m_rowsCollection));
	m_saveLoadFileTimer->setInterval(Helpers::s_minimumRecommendedTimerResolution);
}

void MainWindow::onGenerateRowsButtonClicked()
{
	if (m_rowProvider->isGeneratingProcess())
	{
		m_rowProvider->stopGenerating();
		return;
	}

	m_rowsCollection->clear();

	const int rowsCount = rowsCountSpinBox->value();

	if (rowsCount <= 0)
	{
		QMessageBox::information(this,
			tr("Notification"), 
			tr("Invalid rows count value. Please be sure that value greater than zero."), 
			QMessageBox::Ok
		);

		return;
	}

	m_rowProvider->generateRows(rowsCount);

	saveToFileButton->setEnabled(false);
	loadFromFileButton->setEnabled(false);

	generateRowsButton->setText(tr("Stop Generation"));
}

void MainWindow::onSaveToFileButtonClicked()
{
	ASSERT(!m_saveLoadFileTimer->isActive());
	ASSERT(!m_rowProvider->isGeneratingProcess());

	const QString path = QFileDialog::getSaveFileName(this, tr("Save File"), qApp->applicationDirPath(), QString("*.xml"));

	if (path.isEmpty())
	{
		return;
	}

	tableView->setEnabled(false);

	m_future = std::async(std::launch::async, SerializationTask(m_rowsCollection, path));

	m_saveLoadFileTimer->start();
}

void MainWindow::onLoadFromFileButtonClicked()
{
	ASSERT(!m_saveLoadFileTimer->isActive());
	ASSERT(!m_rowProvider->isGeneratingProcess());

	const QString path = QFileDialog::getOpenFileName(this, tr("Open File"), qApp->applicationDirPath(), QString("*.xml"));

	if (path.isEmpty())
	{
		return;
	}

	m_rowsCollection->clear();

	tableView->setEnabled(false);

	m_future = std::async(std::launch::async, DeserializationTask(m_rowsCollection, path));

	m_saveLoadFileTimer->start();
}

void MainWindow::onGeneratingDone()
{
	saveToFileButton->setEnabled(true);
	loadFromFileButton->setEnabled(true);

	generateRowsButton->setText(tr("Generate"));
}

void MainWindow::onRowDataReady(const RowData& row)
{
	m_rowsCollection->addRow(row);
}

void MainWindow::checkSaveLoadOperationReady()
{
	ASSERT(m_future.valid());

	const std::future_status status = m_future.wait_for(std::chrono::milliseconds(0));

	if (status == std::future_status::timeout)
	{
		return;
	}

	if (status == std::future_status::ready)
	{
		m_future.get();

		tableView->setEnabled(true);

		m_saveLoadFileTimer->stop();
	}
}

}