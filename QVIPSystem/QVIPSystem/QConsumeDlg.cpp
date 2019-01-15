#include "QConsumeDlg.h"
#include <QDateTime>
#include <QComboBox>
QConsumeDlg::QConsumeDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.m_pbOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.m_pbCancle, SIGNAL(clicked()), this, SLOT(onCancle()));
	ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

QConsumeDlg::~QConsumeDlg()
{
	if (m_pCompleter)
	{
		delete m_pCompleter;
		m_pCompleter = NULL;
	}
}


tag_MonetaryRecord QConsumeDlg::getMonetaryRecord() 
{
	return mRecord;
}

void QConsumeDlg::setMap(QMap<QString, int> mList)
{
	ui.comboBox->clear();
	
	for (QMap<QString, int>::iterator its = mList.begin(); its != mList.end(); ++its) {
		ui.comboBox->addItem(its.key(), *its);
	}
	QStringList strList = mList.keys();
	m_pCompleter = new QCompleter(strList, this);
	ui.comboBox->setCompleter(m_pCompleter);
}

Q_SLOT void QConsumeDlg::onOK()
{
	mRecord.nVipNum = ui.comboBox->itemData(ui.comboBox->currentIndex()).toInt();
	mRecord.strName = ui.comboBox->currentText();
	mRecord.nDailyMoney = ui.m_ptotalMeny_lineEdit->text().toDouble();
	mRecord.nDryListCount = ui.m_pdrylist_lineEdit->text().toInt();
	mRecord.strTime = ui.dateTimeEdit->text();
	QDialog::accept();
}

Q_SLOT void QConsumeDlg::onCancle()
{
	QDialog::reject();
}
