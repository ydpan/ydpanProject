#include "QUserAddNew.h"
#include "QMessageBox"
#pragma execution_character_set("utf-8")
QUserAddNew::QUserAddNew(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.m_UserName_lineEdit->clear();
	ui.m_Passwd_lineEdit->clear();
	ui.m_Passwd2_lineEdit->clear();
	ui.m_Power_comboBox;

	connect(ui.m_pbSetDefPwd, SIGNAL(clicked()), this, SLOT(onSetDefaultPwd()));
	connect(ui.m_pbOK, SIGNAL(clicked()), this, SLOT(onSlotOK()));
	connect(ui.m_pbCancel, SIGNAL(clicked()), this, SLOT(onSlotCancle()));
	m_userInfo.mLevel = 0;
	m_userInfo.passwd.clear();
	m_userInfo.strAbout.clear();
	m_userInfo.strName.clear();

	//
	
	for (QMap<int, QString>::const_iterator its = sc_level.begin(); its != sc_level.end(); ++its) {
		ui.m_Power_comboBox->addItem(*its, its.key());
	}
}

QUserAddNew::~QUserAddNew()
{
}

void QUserAddNew::SetFilterList(QStringList &strList)
{
	m_strList.clear();
	m_strList = strList;
}

void QUserAddNew::setUserInfo(UserInfo &userInfo)
{
	m_userInfo = userInfo;
	ui.m_UserName_lineEdit->setDisabled(true);
	ui.m_UserName_lineEdit->setText(userInfo.strName);
	ui.m_Passwd_lineEdit->setText(userInfo.passwd);
	ui.m_Passwd2_lineEdit->setText(userInfo.passwd);
	ui.m_Power_comboBox->setCurrentText(sc_level[userInfo.mLevel]);

}

void QUserAddNew::setCurrentUser(UserInfo &userInfo)
{
	m_CurrentUserInfo = m_userInfo;
}

UserInfo QUserAddNew::getUserInfo()
{
	return m_userInfo;
}

Q_SLOT void QUserAddNew::onSlotOK()
{
	QString strName = ui.m_UserName_lineEdit->text();
	QString passwd1 = ui.m_Passwd_lineEdit->text();
	QString passwd2 = ui.m_Passwd2_lineEdit->text();
	if (strName.isEmpty())
	{
		QMessageBox::warning(this, "Warning", QString("用户名不能为空！"));
		return;
	}
	if (passwd1.isEmpty()|| passwd2.isEmpty())
	{
		QMessageBox::warning(this, "Warning", QString("密码不能为空！"));
		return;
	}
	if (m_strList.contains(strName))
	{
		QMessageBox::warning(this, "Warning", QString("该用户已存在，请另起名字！"));
		return;
	}
	if (passwd1 != passwd2)
	{
		QMessageBox::warning(this, "Warning", QString("两次输入的密码不一致，请重新输入密码！"));
		return;
	}
	m_userInfo.mLevel = ui.m_Power_comboBox->itemData(ui.m_Power_comboBox->currentIndex()).toInt();
	m_userInfo.strName = strName;
	m_userInfo.passwd = passwd1;
	return QDialog::accept();
}

Q_SLOT void QUserAddNew::onSlotCancle()
{
	m_userInfo.mLevel = 0;
	m_userInfo.strName.clear();
	m_userInfo.passwd.clear();
	return QDialog::reject();
}

Q_SLOT void QUserAddNew::onSetDefaultPwd()
{
	ui.m_Passwd_lineEdit->setText("123456");
	ui.m_Passwd2_lineEdit->setText("123456");
}
