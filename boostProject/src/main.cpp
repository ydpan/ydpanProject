#include "boostSeriport.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	boostSeriport w;
	w.show();
	return a.exec();
}
