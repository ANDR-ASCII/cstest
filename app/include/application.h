#pragma once

#include "main_window.h"

namespace Test
{

class Application : public QApplication
{
	Q_OBJECT

public:
	Application(int& argc, char** argv);

private:
	std::unique_ptr<MainWindow> m_mainWindow;
};

}