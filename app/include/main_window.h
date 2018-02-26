#pragma once

#include "ui_main_window.h"

namespace Test
{

class MainWindow : public QMainWindow, protected Ui_MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
};

}