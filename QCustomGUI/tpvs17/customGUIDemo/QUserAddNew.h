#ifndef _QUSERADDNEW_H_
#define _QUSERADDNEW_H_

#include <QDialog>
#include "ui_QUserAddNew.h"
#include "UserLoginDefine.h"
class QUserAddNew : public QDialog
{
	Q_OBJECT

public:
	QUserAddNew(QWidget *parent = Q_NULLPTR);
	~QUserAddNew();

	void SetFilterList(QStringList &strList);//�û����б� ���ڲ����ظ��ַ�
	void setUserInfo(UserInfo &userInfo);//����ӵ��û� ����Ҫ�޸ĵ��û�
	void setCurrentUser(UserInfo &userInfo);//��ǰ��¼���û�  ���ڸ���Ȩ����ʾ��Ӧ����Ϣ

	UserInfo getUserInfo();//��������ӵ��û���Ϣ
	Q_SLOT void onSlotOK();
	Q_SLOT void onSlotCancle();
	Q_SLOT void onSetDefaultPwd();
private:
	Ui::QUserAddNew ui;
	QStringList m_strList;
	UserInfo m_userInfo;
	UserInfo m_CurrentUserInfo;
};


#endif