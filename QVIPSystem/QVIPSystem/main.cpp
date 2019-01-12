#include "QVIPSystem.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//高分屏支持
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	QVIPSystem w;
	w.show();
	return a.exec();
}
