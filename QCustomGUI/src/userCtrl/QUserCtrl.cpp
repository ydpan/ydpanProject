#include "QUserCtrl.h"

QUserCtrl::QUserCtrl(QWidget* parent)
	: m_pMainWnd(parent)
{
	_CurrentUserName.clear();
	m_nLevel = 1;
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
		_pUserDBSqlite->Replace("root", "123456", USER_LEVEL_FULL);
	}	
	
	_pDlgUsersMgr = new QUserManagerDlg(_pUserDBSqlite);
	_pDlgUsersMgr->hide();

	_LoginDlg = new QLoginDlg(_pUserDBSqlite);
	connect(_LoginDlg, SIGNAL(sgLogin(QString, int)), this, SLOT(OnGetUserLevel(QString, int)));
	connect(_LoginDlg, SIGNAL(sgLogout()), this, SLOT(OnLogOutState()));
	_LoginDlg->hide();
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
	_LoginDlg->setCurrentUser(_CurrentUserName);
	_LoginDlg->show();
}

void QUserCtrl::ShowUserMgrDlg()
{
	_pDlgUsersMgr->show();
// 	if (_pDlgUsersMgr->parent() == nullptr)
// 	{
// 		_pDlgUsersMgr->setParent(m_pMainWnd);
// 	}
	
	//_pDlgUsersMgr->ShowByIndex(uci_mgr);
	//m_pDlgUsersMgr->setParent(nullptr);
}

Q_SLOT void QUserCtrl::OnGetUserLevel(QString strName, int nLevel)
{
	_CurrentUserName = strName;
	m_nLevel = nLevel;
}

Q_SLOT void QUserCtrl::OnLogOutState()
{
	_CurrentUserName.clear();
	m_nLevel = 0;
}

