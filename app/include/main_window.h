#pragma once

#include "ui_main_window.h"

namespace Test
{

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
	void onRowDataReady(const QList<QStandardItem*>& row);

private:
	QStandardItemModel* m_itemModel;
	RowProvider* m_rowProvider;
};

}