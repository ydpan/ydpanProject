#include "QUserCtrl.h"
#pragma execution_character_set("utf-8")

QUSERINFO_EXPORT IUserCtrl* UserCtrlCreate() {
	return new QUserCtrl();
}
QUSERINFO_EXPORT void UserCtrlDelete(IUserCtrl* pCtrl)
{
	if (pCtrl) {
		delete pCtrl;
	}
	pCtrl = NULL;
}

QUserCtrl::QUserCtrl(QWidget* parent)
	: m_pMainWnd(parent)
{
	_CurrentUserName.clear();
	m_nLevel = 0;
	QString szPath="";
	
	_pUserDBSqlite = new QUserDB(szPath);
    if (NULL != _pUserDBSqlite)
    {
        if (!_pUserDBSqlite->CreateTable())
        {
            delete _pUserDBSqlite;
			_pUserDBSqlite = NULL;
        }
    }

	auto lst = _pUserDBSqlite->Users(-1);
	if (lst.empty())
	{
		_pUserDBSqlite->Replace("root", "hzleaper", USER_LEVEL_FULL);
	}	
	
	_pDlgUsersMgr = new QUserManagerDlg(_pUserDBSqlite);

	_LoginDlg = new QLoginDlg(_pUserDBSqlite);
	connect(_LoginDlg, SIGNAL(sgLogin(QString, int)), this, SLOT(OnGetUserLevel(QString, int)));

}

QUserCtrl::~QUserCtrl()
{
    if (nullptr != _pUserDBSqlite)
    {
        delete _pUserDBSqlite;
    }

	if (nullptr != _pDlgUsersMgr)
	{
		_pDlgUsersMgr->setParent(nullptr);
		delete _pDlgUsersMgr;
		_pDlgUsersMgr = nullptr;
	}
	if (_LoginDlg) {
		delete _LoginDlg;
	}
}

void QUserCtrl::CheckLogin()
{
	//_LoginDlg->setCurrentUser(_CurrentUserName);
	QStringList strList = _pUserDBSqlite->Users(-1);
	_LoginDlg->AddItems2List(strList);
	_LoginDlg->show();
}

void QUserCtrl::ShowUserMgrDlg()
{
	_pDlgUsersMgr->SetUserAndLevel(_CurrentUserName, m_nLevel);
	_pDlgUsersMgr->OnUpdateUserList();
	_pDlgUsersMgr->show();
}

void QUserCtrl::LogOutUser()
{
	_CurrentUserName.clear();
	m_nLevel = 0;
	m_State = EM_LOGOUT;
	emit(sgCurrentUserInfo(_CurrentUserName, m_nLevel, m_State));
}

Q_SLOT void QUserCtrl::OnGetUserLevel(QString strName, int nLevel)
{
	_CurrentUserName = strName;
	m_nLevel = nLevel;
	m_State = EM_LOGIN;
	emit(sgCurrentUserInfo(_CurrentUserName, m_nLevel, m_State));
}


