#include "application.h"

int main(int argc, char** argv)
{
	Test::Application app(argc, argv);
	return app.exec();
}