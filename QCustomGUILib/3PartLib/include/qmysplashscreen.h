#ifndef QMYSPLASHSCREEN_H
#define QMYSPLASHSCREEN_H

#include <QSplashScreen>
#include "QProgressBar"
#include "QList"
#include "QTextEdit"
class QMySplashScreen : public QSplashScreen
{
	Q_OBJECT

public:
	QMySplashScreen(const QPixmap & pixmap = QPixmap(),int time=100);
	~QMySplashScreen();
	Q_SLOT void ShowMsg(QString str);
private:
	int elapseTime;
	QTextEdit *pEdit;
};

#endif // QMYSPLASHSCREEN_H
