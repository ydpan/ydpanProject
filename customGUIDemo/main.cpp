#include "customGUIDemo.h"
#include <QtWidgets/QApplication>
#include "DarkStyle.h"
#include "framelesswindow.h"
int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//高分屏支持
	QApplication::addLibraryPath("./plugins");
	QApplication::setStyle(new DarkStyle);//黑色分格
	QApplication a(argc, argv);
	customGUIDemo w;// = new customGUIDemo;
	//FramelessWindow framw;//该类存在bug
	//framw.setContent(w);
	//framw.show();
	w.show();
	return a.exec();
}
