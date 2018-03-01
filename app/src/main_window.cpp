#include "main_window.h"
#include "row_provider.h"

namespace Test
{

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_itemModel(new QStandardItemModel(this))
	, m_rowProvider(new RowProvider(this))
{
	setupUi(this);

	VERIFY(connect(generateRowsButton, &QPushButton::clicked, this, &MainWindow::onGenerateRowsButtonClicked));
	VERIFY(connect(saveToFileButton, &QPushButton::clicked, this, &MainWindow::onSaveToFileButtonClicked));
	VERIFY(connect(loadFromFileButton, &QPushButton::clicked, this, &MainWindow::onLoadFromFileButtonClicked));
	VERIFY(connect(m_rowProvider, &RowProvider::rowReady, this, &MainWindow::onRowDataReady));
	VERIFY(connect(m_rowProvider, &RowProvider::generatingDone, this, &MainWindow::onGeneratingDone));

	tableView->setModel(m_itemModel);
}

void MainWindow::onGenerateRowsButtonClicked()
{
	if (m_rowProvider->isGeneratingProcess())
	{
		m_rowProvider->stopGenerating();
		return;
	}

	m_itemModel->clear();

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

	generateRowsButton->setText(tr("Cancel Generation"));
}

void MainWindow::onSaveToFileButtonClicked()
{
	const QString path = QFileDialog::getSaveFileName(this, tr("Save File"), qApp->applicationDirPath(), QString("*.xml"));

	if (path.isEmpty())
	{
		return;
	}
}

void MainWindow::onLoadFromFileButtonClicked()
{
	const QString path = QFileDialog::getOpenFileName(this, tr("Open File"), qApp->applicationDirPath(), QString("*.xml"));

	if (path.isEmpty())
	{
		return;
	}
}

void MainWindow::onGeneratingDone()
{
	generateRowsButton->setText(tr("Generate"));
}

void MainWindow::onRowDataReady(const RowData& row)
{
	QList<QStandardItem*> rowItems;

	rowItems.push_back(new QStandardItem(QString(row.string)));
	rowItems.push_back(new QStandardItem(QString::fromUtf8("%1").arg(row.number)));
	rowItems.push_back(new QStandardItem(QString::fromUtf8("%1").arg(row.floatingPointNumber, 0, 'g', 3)));
	rowItems.push_back(new QStandardItem(QString::fromUtf8("%1").arg(row.boolValue ? "true" : "false")));

	m_itemModel->appendRow(rowItems);
}

}