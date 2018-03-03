#pragma once

#include "ui_main_window.h"

namespace Test
{

struct RowData;
class RowProvider;
class RowsCollection;

class MainWindow : public QMainWindow, protected Ui_MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

private slots:
	void onGenerateRowsButtonClicked();
	void onSaveToFileButtonClicked();
	void onLoadFromFileButtonClicked();
	void onGeneratingDone();
	void onRowDataReady(const RowData& row);
	void checkSaveLoadOperationReady();
	void onRowsCollectionChanged();
	void currentSelectedRowChanged(const QModelIndex& current, const QModelIndex& previous);

private:
	RowProvider* m_rowProvider;
	std::shared_ptr<RowsCollection> m_rowsCollection;
	QTimer* m_saveLoadFileTimer;
	std::future<void> m_future;
};

}