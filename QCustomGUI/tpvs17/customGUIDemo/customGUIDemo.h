#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_customGUIDemo.h"
#include "GridLayout.h"
#include "GridTray.h"

#include <QSharedpointer>

#include <QTimer>
#include "QUserCtrl.h"

class customGUIDemo : public QMainWindow
{
	Q_OBJECT

public:
	customGUIDemo(QWidget *parent = Q_NULLPTR);
	~customGUIDemo();
	template<typename _Widget>
	_Widget* replaceWidget(QWidget *pWidget);
	void InitTrayLayout();

public :
	Q_SLOT void onTimer();
	Q_SLOT void onTimer2();
	Q_SLOT void onClicked();
private:
	Ui::customGUIDemoClass ui;
	CGridLayoutView *m_pgridView{ nullptr };
	QSharedPointer<CGridProject> _pCGridProject;
	QTimer m_timer;
	QTimer m_timer2;

	QUserCtrl *_pUserCtrl{ nullptr };
};


