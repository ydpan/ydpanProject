#include "boostSeriport.h"
#include <QtWidgets/QApplication>
#include "QUdpTestUI.h"
int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	/*boostSeriport w;*/
	QUdpTestUI w;
	w.show();
	return a.exec();
}
