#pragma once

#include "ui_main_window.h"

namespace Test
{

class RowDataProvider;
struct RowData;

class MainWindow : public QMainWindow, protected Ui_MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

private slots:
	void onGenerateRowsButtonClicked();
	void onSaveToFileButtonClicked();
	void onLoadFromFileButtonClicked();
	void onRowDataReady(const RowData& rowData);

private:
	QStandardItemModel* m_itemModel;
	RowDataProvider* m_rowDataProvider;
};

}