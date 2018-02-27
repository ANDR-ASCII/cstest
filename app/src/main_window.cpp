#include "main_window.h"
#include "row_data_provider.h"
#include "generate_row_task.h"

namespace Test
{

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_itemModel(new QStandardItemModel(this))
	, m_rowDataProvider(new RowDataProvider(this))
{
	setupUi(this);

	VERIFY(connect(generateRowsButton, &QPushButton::clicked, this, &MainWindow::onGenerateRowsButtonClicked));
	VERIFY(connect(saveToFileButton, &QPushButton::clicked, this, &MainWindow::onSaveToFileButtonClicked));
	VERIFY(connect(loadFromFileButton, &QPushButton::clicked, this, &MainWindow::onLoadFromFileButtonClicked));
	VERIFY(connect(m_rowDataProvider, &RowDataProvider::rowDataReady, this, &MainWindow::onRowDataReady));

	tableView->setModel(m_itemModel);
}

void MainWindow::onGenerateRowsButtonClicked()
{
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

	m_rowDataProvider->generateRowsData(rowsCount);
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

void MainWindow::onRowDataReady(const RowData& rowData)
{
	QList<QStandardItem*> rowItems;

	rowItems.push_back(new QStandardItem(rowData.string));
	rowItems.push_back(new QStandardItem(QString::fromUtf8("%1").arg(rowData.number)));
	rowItems.push_back(new QStandardItem(QString::fromUtf8("%1").arg(rowData.floatingPointNumber)));
	rowItems.push_back(new QStandardItem(rowData.boolValue ? QString::fromUtf8("true") : QString::fromUtf8("false")));

	m_itemModel->appendRow(rowItems);
}

}