#include "application.h"
#include "helpers.h"

int main(int argc, char** argv)
{
	Test::Application app(argc, argv);
	return app.exec();
}