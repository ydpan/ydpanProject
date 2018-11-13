#include "QLoginDlg.h"
#include <QMessageBox>
/*
登陆窗口有固定按钮启动（login按钮）

登陆功能：
需根据用户登陆信息提示，
（1）用户名不存在
（2）登陆失败，密码错误
（3）登陆密码为空
（4）打开登陆窗口时，显示昨天登陆的用户作为备选
登陆成功，直接返回用户名及权限等级给上一级UI

注销功能：
如已登陆成功，需显示用户名，密码需重新输入；若登陆失败，显示的用户名应为空

取消功能：
直接关闭窗口，不做任何反应

*/
QLoginDlg::QLoginDlg(QUserDB* pUserDB)
	: _pUserDB(pUserDB)
{
	ui.setupUi(this);
	_UserNameComboBox = ui.login_UserName;
	_PasswdLineEdit = ui.login_Passwd;
	_UserNameComboBox->setEditable(true);
	
	_PasswdLineEdit->setDisabled(true);

	connect(_UserNameComboBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(OnEditTextChanged(const QString &)));
// 	QStringList strList;
// 	strList << "root" << "testUser" << "test";
// 	AddItems2List(strList);
// 	_UserNameComboBox->setCurrentIndex(-1);

	connect(ui.login_OKpb, SIGNAL(clicked()), this, SLOT(OnClicked()));
	connect(ui.login_Logoutpb, SIGNAL(clicked()), this, SLOT(OnClicked()));
	connect(ui.login_Cancelpb, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

QLoginDlg::~QLoginDlg()
{

}

Q_SLOT void QLoginDlg::OnEditTextChanged(const QString &text)
{
	if (!text.isEmpty())
		_PasswdLineEdit->setDisabled(false);
	else
		_PasswdLineEdit->setDisabled(true);
	_PasswdLineEdit->clear();
}

Q_SLOT void QLoginDlg::OnClicked()
{
	QString strObj = sender()->objectName();
	if (strObj == "login_OKpb") {
		QString password;
		QString strUser;
		strUser = _UserNameComboBox->currentText();
		password = _PasswdLineEdit->text();

		if (strUser.isEmpty())
		{
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#ff0000;>用户名不能为空！！！< / span>< / p>< / body>< / html>");
			QMessageBox::information(this, "警告", strText, QMessageBox::Ok);
			return;
		}
		if (password.isEmpty())
		{
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#ff0000;>密码不能为空！！！< / span>< / p>< / body>< / html>");
			QMessageBox::information(this, "警告", strText, QMessageBox::Ok);
			return;
		}

		int nLevel = 0;
		if (_pUserDB->Login(strUser, password, nLevel))
		{
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#00ff00;>登录成功< / span>< / p>< / body>< / html>");
			_pUserDB->CurUser();
			_PasswdLineEdit->clear();
			emit(sgLogin(strUser, nLevel));
			accept();
		}
		else
		{
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#ff0000;>密码错误！！！< / span>< / p>< / body>< / html>");
			QMessageBox::information(this, "Error", strText, QMessageBox::Ok);
		}
	}
	else if (strObj == "login_Logoutpb") {
		_pUserDB->Logout();
		emit(sgLogout());
		accept();
	}
	else if (strObj == "login_Cancelpb") {
		close();
	}
}

void QLoginDlg::AddItems2List(QStringList strList)
{
	if (_UserNameComboBox) {
		_UserNameComboBox->clear();
		_UserNameComboBox->clearEditText();
		_UserNameComboBox->addItems(strList);
	}
}

void QLoginDlg::setCurrentUser(QString str)
{
	_UserNameComboBox->setCurrentText(str);
}
