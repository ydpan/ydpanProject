#include "SDKTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SDKTest w;
	w.show();
	return a.exec();
}
