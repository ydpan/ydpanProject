#ifndef _QUSERCTRL_H_
#define _QUSERCTRL_H_

#include "QUserDB.h"
#include "QUserManagerDlg.h"
#include "QLoginDlg.h"
#include "QUserBase.h"
#include "quserinfo_global.h"
class QUserCtrl :public IUserCtrl
{
	Q_OBJECT
public:
	QUserCtrl(QWidget* parent = 0);
    ~QUserCtrl();

    void CheckLogin();	
	void ShowUserMgrDlg();
	void LogOutUser();
	QString CurUser() { return _CurrentUserName; }			//当前登录用户的用户名
	int CurUserLevel() { return m_nLevel; }					//当前登录用户的等级
	int getLoginState() { return (int)m_State; }			//查询登录状态
private:
	Q_SLOT void OnGetUserLevel(QString strName, int nLevel);
public:
    QString				_CurrentUserName;
	int					m_nLevel{ 0 };
	LogIndState			m_State{ EM_LOGOUT };
//signals:
//	void sgCurrentUserInfo(QString userName, int level,int state);
private:
	QWidget*			m_pMainWnd{ nullptr };
    QUserDB*			_pUserDBSqlite{ nullptr };
	QUserManagerDlg*	_pDlgUsersMgr{ nullptr };
	QLoginDlg*			_LoginDlg{ nullptr };
};

#endif
