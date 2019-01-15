#ifndef _QCONSUMEDLG_H_
#define _QCONSUMEDLG_H_

#include <QDialog>
#include "ui_QConsumeDlg.h"
#include "baseStructDefine.h"
class QConsumeDlg : public QDialog
{
	Q_OBJECT

public:
	QConsumeDlg(QWidget *parent = Q_NULLPTR);
	~QConsumeDlg();

	tag_MonetaryRecord getMonetaryRecord();

	Q_SLOT void onOK();
	Q_SLOT void onCancle();
private:
	Ui::QConsumeDlg ui;
	tag_MonetaryRecord mRecord;
};

#endif
