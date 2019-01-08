#ifndef _LOGINMODIFY_H_
#define _LOGINMODIFY_H_

#include <QDialog>
#include "ui_QLoginModify.h"

class QLoginModify : public QDialog
{
	Q_OBJECT

public:
	QLoginModify(QWidget *parent = Q_NULLPTR);
	~QLoginModify();
	void SetUserName(QString strName);
	Q_SLOT void onClicked();
private:
	Ui::QLoginModify ui;
};

#endif

