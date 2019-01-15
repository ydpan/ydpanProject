#ifndef _QCONSUMEDLG_H_
#define _QCONSUMEDLG_H_

#include <QDialog>
#include "ui_QConsumeDlg.h"
#include "baseStructDefine.h"
#include <QCompleter>
class QConsumeDlg : public QDialog
{
	Q_OBJECT

public:
	QConsumeDlg(QWidget *parent = Q_NULLPTR);
	~QConsumeDlg();

	tag_MonetaryRecord getMonetaryRecord();
	void setMap(QMap<QString, int> mList);
	Q_SLOT void onOK();
	Q_SLOT void onCancle();
private:
	Ui::QConsumeDlg ui;
	tag_MonetaryRecord mRecord;
	QCompleter *m_pCompleter{NULL};
};

#endif
