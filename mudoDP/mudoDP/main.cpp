#include "mudoDP.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mudoDP w;
	w.show();
	return a.exec();
}
