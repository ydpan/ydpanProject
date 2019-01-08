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

	void SetFilterList(QStringList &strList);//用户名列表 用于查找重复字符
	void setUserInfo(UserInfo &userInfo);//新添加的用户 或需要修改的用户
	void setCurrentUser(UserInfo &userInfo);//当前登录的用户  用于根据权限显示对应的信息

	UserInfo getUserInfo();//或许新添加的用户信息
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