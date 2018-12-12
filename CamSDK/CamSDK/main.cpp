#include "CamSDK.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CamSDK w;
	w.show();
	return a.exec();
}
