#include "qmysplashscreen.h"
#include "QTimer"
#include "QDateTime"
#include "QTextEdit"

#pragma execution_character_set("utf-8")

QMySplashScreen::QMySplashScreen(const QPixmap & pixmap, int time) :QSplashScreen(pixmap), elapseTime(time)
{
	pEdit = new QTextEdit(this);
	QPixmap m_map(":/image/Resource/windows4.png");
	setPixmap(m_map);
	pEdit->setGeometry(30, m_map.height() / 2, m_map.width(), m_map.height() / 2 - 20);
	pEdit->setAttribute(Qt::WA_TranslucentBackground, true);
	pEdit->setStyleSheet("background-color: rgb(255, 255, 255, 60);color:#1E296B;border:0px solid gray;");

	QPalette pl = pEdit->palette();
	pl.setBrush(QPalette::Base, QBrush(QColor(255, 0, 0, 0)));
	
	pEdit->setPalette(pl);
	pEdit->append("程序正在启动...");
}

QMySplashScreen::~QMySplashScreen()
{
	if (pEdit)
 	{
		delete pEdit;
		pEdit = NULL;
 	}
}

void QMySplashScreen::ShowMsg(QString str)
{
	if (pEdit)
		pEdit->append(str);
	Qt::Alignment topRight = Qt::AlignLeft | Qt::AlignBottom;
	showMessage(str, topRight, Qt::blue);
}
