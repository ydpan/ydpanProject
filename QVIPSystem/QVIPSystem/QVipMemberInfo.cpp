#include "QVipMemberInfo.h"

QVipMemberInfo::QVipMemberInfo(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint);
	ui.m_pVipNumber_lineEdit;
	ui.m_pName_lineEdit;
	ui.m_pPhoneNum_lineEdit;
	ui.m_pChatName_lineEdit;
	ui.m_pAddress_lineEdit;
	connect(ui.m_pbOK,SIGNAL(clicked()),this,SLOT(onOK()));
	connect(ui.m_pbCancle,SIGNAL(clicked()),this,SLOT(onCancle()));
}

QVipMemberInfo::~QVipMemberInfo()
{

}

void QVipMemberInfo::setBestID(int nID)
{
	ui.m_pVipNumber_lineEdit->setText(QString::number(nID));
}

Q_SLOT void QVipMemberInfo::onOK()
{
	m_Info.nVipNum = ui.m_pVipNumber_lineEdit->text().toInt();
	m_Info.strName = ui.m_pName_lineEdit->text();
	m_Info.nPhoneNumber = ui.m_pPhoneNum_lineEdit->text();
	m_Info.strAddress = ui.m_pAddress_lineEdit->text();
	m_Info.strWeixinName = ui.m_pChatName_lineEdit->text();
	QDialog::accept();
}

Q_SLOT void QVipMemberInfo::onCancle()
{
	QDialog::reject();
}
