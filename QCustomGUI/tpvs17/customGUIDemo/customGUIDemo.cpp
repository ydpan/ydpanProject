#include "customGUIDemo.h"
#include "QSearchEdit.h"
#include <QLayout>
#pragma execution_character_set("utf-8")
customGUIDemo::customGUIDemo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
 	QWidget *widget = ui.widget;
 	//QSearchEdit *p = replaceWidget<QSearchEdit>(widget);
 	QLineEdit *pwidg = ui.lineEdit;
 	//QSearchEdit *pp = new QSearchEdit;
 	//pp->show();
 	QSearchEdit *p = replaceWidget<QSearchEdit>(pwidg);

	InitTrayLayout();

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	connect(&m_timer2, SIGNAL(timeout()), this, SLOT(onTimer2()));
	m_timer.start(500);
	m_timer2.start(500);

	connect(ui.m_pbLogin_Out, SIGNAL(clicked()), this, SLOT(onClicked()));
	connect(ui.m_pbUserMgr, SIGNAL(clicked()), this, SLOT(onClicked()));
	_pUserCtrl = new QUserCtrl(this);
	connect(_pUserCtrl, SIGNAL(sgCurrentUserInfo(QString, int, int)), this, SLOT(onLogInOut(QString, int, int)));

	ui.pushButton->setVisible(false);
	ui.pushButton_2->setVisible(false);
	ui.pushButton_3->setVisible(false);
}

customGUIDemo::~customGUIDemo()
{
	if (_pUserCtrl) {
		delete _pUserCtrl;
	}
}

template<typename _Widget>
_Widget* customGUIDemo::replaceWidget(QWidget *pSrcWidget)
{
/*	QWidget* pSrcWidget = GET_WIDGET_POINTER(name, QWidget);*/
	if (!pSrcWidget) {
		return nullptr;
	}
	QWidget* pParent = qobject_cast<QWidget*>(pSrcWidget->parent());
	if (!pParent) {
		return nullptr;
	}
	
	QLayout *pLayout = pParent->layout();
	if (!pLayout)
		return nullptr;

	_Widget* pDstWidget = new _Widget;
	auto *pFrom = pLayout->replaceWidget(pSrcWidget, pDstWidget);
	delete pFrom;
	delete pSrcWidget;
	return pDstWidget;
}

void customGUIDemo::InitTrayLayout()
{
	m_pgridView = replaceWidget<CGridLayoutView>(ui.widget);
	_pCGridProject = QSharedPointer<CGridProject>(new CGridProject);
	_pCGridProject->LoadAndInit("C:\\1Documents\\PersonProject\\MyQtCustomLib\\layout_tray.txt");
	if (m_pgridView)
		m_pgridView->setScene(_pCGridProject->getScenePtr());
}

Q_SLOT void customGUIDemo::onTimer()
{
	static int testI = 0;
	static int testJ = 0;
	static int bTranslate = 0;
	emGridItemResult rlt = GRID_DETECTED_A;

	if (bTranslate % 2 == 1)
		rlt = GRID_DETECTED_A;
	else if (bTranslate % 3 == 0)
		rlt = GRID_DETECTED_B;
	else if (bTranslate % 4 == 0)
		rlt = GRID_DETECTED_NG;
	else if (bTranslate % 5 == 0)
		rlt = GRID_DETECTED_NULL;
	else
		rlt = GRID_NO_DETECT;

	if (testI < _pCGridProject->getCols())
	{
		if (testJ < _pCGridProject->getRows())
		{
			_pCGridProject->updateItem(testJ, testI, rlt);
			testJ++;
		}
		else
		{
			testJ = 0;
			testI++;
			_pCGridProject->updateItem(testJ, testI, rlt);
		}
	}
	else
	{
		testJ = 0;
		testI = 0;
		_pCGridProject->updateItem(testJ, testI, rlt);
		bTranslate++;
	}
	
}

Q_SLOT void customGUIDemo::onTimer2()
{
// 	int testI = 3;
// 	int testJ = 2;
// 	static int i = 0;
// 
// 	// 	static int b = 0;
// 	// 	b = b + 1;
// 	// 	testI = b + testI;
// 	i++;
// 	if (i % 2 == 0)
// 		_pCGridProject->updateItem(1, 1, GRID_DETECTED_A);
// 	else
// 		_pCGridProject->updateItem(1, 1, GRID_DETECTED_B);
// 	if (i % 2 == 0)
// 		_pCGridProject->updateItem(7, 5, GRID_DETECTED_A);
// 	else
// 		_pCGridProject->updateItem(7, 5, GRID_DETECTED_NG);
// 	if (i % 3 == 0)
// 		_pCGridProject->updateItem(6, 1, GRID_DETECTED_A);
// 	else
// 		_pCGridProject->updateItem(6, 1, GRID_DETECTED_NG);
// 	if (i % 4 == 0)
// 		_pCGridProject->updateItem(1, 9, GRID_DETECTED_A);
// 	else
// 		_pCGridProject->updateItem(1, 9, GRID_DETECTED_B);
// 	if (i % 5 == 0)
// 		_pCGridProject->updateItem(2, 7, GRID_DETECTED_NULL);
// 	else
// 		_pCGridProject->updateItem(2, 7, GRID_DETECTED_NG);

	static int testI = _pCGridProject->getRows()-1; 
	static int testJ = _pCGridProject->getCols()-1;
	static int bTranslate = 0;
	emGridItemResult rlt = GRID_DETECTED_A;

	if (bTranslate % 2 == 1)
		rlt = GRID_DETECTED_NG;
	else if (bTranslate % 3 == 0)
		rlt = GRID_DETECTED_B;
	else if (bTranslate % 4 == 0)
		rlt = GRID_DETECTED_NULL;
	else if (bTranslate % 5 == 0)
		rlt = GRID_NO_DETECT;
	else
		rlt = GRID_DETECTED_A;

	if (testI >= 0)
	{
		if (testJ >= 0)
		{
			_pCGridProject->updateItem(testI, testJ, rlt);
			testJ--;
		}
		else
		{
			testJ = _pCGridProject->getCols()-1;
			testI--;
			_pCGridProject->updateItem(testI, testJ, rlt);
		}
	}
	else
	{
		testJ = _pCGridProject->getCols()-1;
		testI = _pCGridProject->getRows()-1;
		_pCGridProject->updateItem(testI, testJ, rlt);
		bTranslate++;
	}
}

Q_SLOT void customGUIDemo::onClicked()
{
	QString strObj = sender()->objectName();
	if (strObj == "m_pbLogin_Out") {
		if (_pUserCtrl) {
			if (_pUserCtrl->getLoginState() == EM_LOGIN)
				_pUserCtrl->LogOutUser();
			else
				_pUserCtrl->CheckLogin();
		}
	}
	else if (strObj == "m_pbUserMgr") {
		if (_pUserCtrl) {
			_pUserCtrl->ShowUserMgrDlg();
		}
	}
}

Q_SLOT void customGUIDemo::onLogInOut(QString strName, int level, int state)
{
	ui.label->setText(strName);
	if (state == 0) {
		if (level == 1) {
				ui.pushButton->setVisible(true);
		}
		if (level == 9) {
			ui.pushButton->setVisible(true);
			ui.pushButton_2->setVisible(true);
			ui.pushButton_3->setVisible(true);
		}
		ui.m_pbLogin_Out->setText("×¢Ïú");
	}
	else
	{
		ui.pushButton->setVisible(false);
		ui.pushButton_2->setVisible(false);
		ui.pushButton_3->setVisible(false);
		ui.m_pbLogin_Out->setText("µÇÂ¼");
	}
}
