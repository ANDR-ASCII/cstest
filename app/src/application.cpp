#include "application.h"

namespace Test
{

Application::Application(int& argc, char** argv)
	: QApplication(argc, argv)
	, m_mainWindow(std::make_unique<MainWindow>())
{
	m_mainWindow->show();
}

}