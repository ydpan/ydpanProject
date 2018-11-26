#include "customGUIDemo.h"
#include <QtWidgets/QApplication>
#include "DarkStyle.h"
#include "framelesswindow.h"
int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//�߷���֧��
	QApplication::addLibraryPath("./plugins");
	QApplication::setStyle(new DarkStyle);//��ɫ�ָ�
	QApplication a(argc, argv);
	customGUIDemo w;// = new customGUIDemo;
	//FramelessWindow framw;//�������bug
	//framw.setContent(w);
	//framw.show();
	w.show();
	return a.exec();
}
