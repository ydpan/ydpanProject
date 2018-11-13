#include "QLoginDlg.h"
#include <QMessageBox>
/*
��½�����й̶���ť������login��ť��

��½���ܣ�
������û���½��Ϣ��ʾ��
��1���û���������
��2����½ʧ�ܣ��������
��3����½����Ϊ��
��4���򿪵�½����ʱ����ʾ�����½���û���Ϊ��ѡ
��½�ɹ���ֱ�ӷ����û�����Ȩ�޵ȼ�����һ��UI

ע�����ܣ�
���ѵ�½�ɹ�������ʾ�û������������������룻����½ʧ�ܣ���ʾ���û���ӦΪ��

ȡ�����ܣ�
ֱ�ӹرմ��ڣ������κη�Ӧ

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
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#ff0000;>�û�������Ϊ�գ�����< / span>< / p>< / body>< / html>");
			QMessageBox::information(this, "����", strText, QMessageBox::Ok);
			return;
		}
		if (password.isEmpty())
		{
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#ff0000;>���벻��Ϊ�գ�����< / span>< / p>< / body>< / html>");
			QMessageBox::information(this, "����", strText, QMessageBox::Ok);
			return;
		}

		int nLevel = 0;
		if (_pUserDB->Login(strUser, password, nLevel))
		{
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#00ff00;>��¼�ɹ�< / span>< / p>< / body>< / html>");
			_pUserDB->CurUser();
			_PasswdLineEdit->clear();
			emit(sgLogin(strUser, nLevel));
			accept();
		}
		else
		{
			QString strText = QString::fromLocal8Bit("<html><head / ><body><p><span style = color:#ff0000;>������󣡣���< / span>< / p>< / body>< / html>");
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
