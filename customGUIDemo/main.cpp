#include "customGUIDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	customGUIDemo w;
	w.show();
	return a.exec();
}
