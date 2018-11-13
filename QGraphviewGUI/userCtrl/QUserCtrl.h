#ifndef _QUSERCTRL_H_
#define _QUSERCTRL_H_

#include "QUserDB.h"
#include "QUserManagerDlg.h"
#include "QLoginDlg.h"

class QUserCtrl :public QObject
{
	Q_OBJECT
public:
	QUserCtrl(QWidget* parent = 0);
    ~QUserCtrl();

    void CheckLogin();	
	void ShowUserMgrDlg();

	QString CurUser() { return _CurrentUserName; }
	int CurUserLevel() { return m_nLevel; }
	
private:
	Q_SLOT void OnGetUserLevel(QString strName, int nLevel);
	Q_SLOT void OnLogOutState();
public:
    QString          _CurrentUserName;
	int              m_nLevel{ 0 };

private:
	QWidget*			m_pMainWnd{ nullptr };
    QUserDB*			_pUserDBSqlite{ nullptr };
	QUserManagerDlg*	_pDlgUsersMgr{ nullptr };
	QLoginDlg*			_LoginDlg{ nullptr };
};

#endif
