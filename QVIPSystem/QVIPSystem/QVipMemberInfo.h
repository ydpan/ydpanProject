#ifndef _QVIPMEMBERINFO_H_
#define _QVIPMEMBERINFO_H_

#include <QDialog>
#include "ui_QVipMemberInfo.h"
#include "baseStructDefine.h"
class QVipMemberInfo : public QDialog
{
	Q_OBJECT

public:
	QVipMemberInfo(QWidget *parent = Q_NULLPTR);
	~QVipMemberInfo();
	void setBestID(int nID);
	void setModifyMoney();
	void setInfo(VipMemberInfo &info);
	tag_VipMemberInfo getNewInfo() { return m_Info; };
	Q_SLOT void onOK();
	Q_SLOT void onCancle();
private:
	Ui::QVipMemberInfo ui;
	tag_VipMemberInfo m_Info;
};

#endif
