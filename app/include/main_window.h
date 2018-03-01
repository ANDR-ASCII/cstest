#pragma once

#include "ui_main_window.h"

namespace Test
{

struct RowData;
class RowProvider;

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

private:
	QStandardItemModel* m_itemModel;
	RowProvider* m_rowProvider;
};

}