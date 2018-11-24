#include "boostSeriport.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	boostSeriport w;
	w.show();
	return a.exec();
}
