#include "QConsumeDlg.h"
#include <QDateTime>
QConsumeDlg::QConsumeDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.m_pbOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.m_pbCancle, SIGNAL(clicked()), this, SLOT(onCancle()));
	;
	ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

QConsumeDlg::~QConsumeDlg()
{
}


tag_MonetaryRecord QConsumeDlg::getMonetaryRecord() 
{
	return mRecord;
}

Q_SLOT void QConsumeDlg::onOK()
{
	mRecord.nVipNum = ui.m_pVipNumber_lineEdit->text().toInt();
	mRecord.strName = ui.m_pName_lineEdit->text();
	mRecord.nDailyMoney = ui.m_ptotalMeny_lineEdit->text().toDouble();
	mRecord.nDryListCount = ui.m_pdrylist_lineEdit->text().toInt();
	mRecord.strTime = ui.dateTimeEdit->text();
	QDialog::accept();
}

Q_SLOT void QConsumeDlg::onCancle()
{
	QDialog::reject();
}
