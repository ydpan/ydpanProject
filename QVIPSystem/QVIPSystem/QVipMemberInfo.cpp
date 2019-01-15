#include "QVipMemberInfo.h"
#pragma execution_character_set("utf-8")
QVipMemberInfo::QVipMemberInfo(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint);
	ui.m_pVipNumber_lineEdit;
	ui.m_pName_lineEdit;
	ui.m_pPhoneNum_lineEdit;
	ui.m_pAddress_lineEdit;
	setWindowTitle("VIP成员信息添加");
	connect(ui.m_pbOK,SIGNAL(clicked()),this,SLOT(onOK()));
	connect(ui.m_pbCancle,SIGNAL(clicked()),this,SLOT(onCancle()));

	ui.m_ptotalMeny_lineEdit->setVisible(false);
	ui.m_pdrylist_lineEdit->setVisible(false);
	ui.m_pIntertion_lineEdit->setVisible(false);
	ui.label_4->setVisible(false);
	ui.label_6->setVisible(false);
	ui.label_7->setVisible(false);
}

QVipMemberInfo::~QVipMemberInfo()
{

}

void QVipMemberInfo::setBestID(int nID)
{
	ui.m_pVipNumber_lineEdit->setText(QString::number(nID));

}

void QVipMemberInfo::setModifyMoney()
{
	ui.m_ptotalMeny_lineEdit->setVisible(true);
	ui.m_pdrylist_lineEdit->setVisible(true);
	ui.m_pIntertion_lineEdit->setVisible(true);
	ui.label_4->setVisible(true);
	ui.label_6->setVisible(true);
	ui.label_7->setVisible(true);

	ui.label_3->setVisible(false);
	ui.m_pPhoneNum_lineEdit->setVisible(false);
	ui.label_5->setVisible(false);
	ui.m_pAddress_lineEdit->setVisible(false);

	ui.m_pVipNumber_lineEdit->setDisabled(true);
	ui.m_pName_lineEdit->setDisabled(true);
}

void QVipMemberInfo::setInfo(VipMemberInfo &info)
{
	ui.m_pVipNumber_lineEdit->setText(QString::number(info.nVipNum));
	ui.m_pName_lineEdit->setText(info.strName);
	ui.m_pPhoneNum_lineEdit->setText(info.nPhoneNumber);
	ui.m_pAddress_lineEdit->setText(info.strAddress);
	ui.m_ptotalMeny_lineEdit->setText(QString("%1").arg(info.nMonetary));
	ui.m_pdrylist_lineEdit->setText(QString("%1").arg(info.nDrying_list));
	ui.m_pIntertion_lineEdit->setText(QString("%1").arg(info.nIntegration));
	m_Info = info;
}

Q_SLOT void QVipMemberInfo::onOK()
{
	m_Info.nVipNum = ui.m_pVipNumber_lineEdit->text().toInt();
	m_Info.strName = ui.m_pName_lineEdit->text();
	m_Info.nPhoneNumber = ui.m_pPhoneNum_lineEdit->text();
	m_Info.strAddress = ui.m_pAddress_lineEdit->text();
	m_Info.nDrying_list = ui.m_pdrylist_lineEdit->text().toInt();
	m_Info.nMonetary = ui.m_ptotalMeny_lineEdit->text().toDouble();
	m_Info.nIntegration = ui.m_pIntertion_lineEdit->text().toDouble();
	QDialog::accept();
}

Q_SLOT void QVipMemberInfo::onCancle()
{
	QDialog::reject();
}
