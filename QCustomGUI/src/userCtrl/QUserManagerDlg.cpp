#include "QUserManagerDlg.h"
#include <QMessageBox>
#include "QUserAddNew.h"
#include "QUserBase.h"
#pragma execution_character_set("utf-8")

QString levelName(int nLevel) {
	QString strLevel = QString("无用户");
	if (sc_level.contains(nLevel))
		strLevel = sc_level[nLevel];
	return strLevel;
}

QUserManagerDlg::QUserManagerDlg(QUserDB* pLogic)
	: QDialog(nullptr)
	, m_pUserDB(pLogic)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowIcon(QIcon(":/userinfo"));
	QStringList headerList;
	headerList << "用户名" << "等级";
	plistModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel);
	plistModel->setHorizontalHeaderLabels(headerList);
	ui.tableView->setModel(plistModel.data());
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(ui.m_pbAddUser,SIGNAL(clicked()),this,SLOT(onAddUser()));
	connect(ui.m_pbDelUser, SIGNAL(clicked()), this, SLOT(onDelUser()));
	connect(ui.m_pbModifyUser, SIGNAL(clicked()), this, SLOT(onModifyUser()));
	ui.label_Status->setText("");
}

QUserManagerDlg::~QUserManagerDlg()
{
}

void QUserManagerDlg::SetUserAndLevel(QString strName, int level)
{
	m_strName = strName; 
	m_level = level;
	ui.label_UserName->setText(m_strName);
	if (m_strName.isEmpty())
		ui.label_UserName->setText("未登录");
	ui.label_Status->setText("");
}


void QUserManagerDlg::onAddUser()
{
	if (m_strName.isEmpty())
	{
		QMessageBox::warning(this, "错误警告", QString("你还没登录，请登录或请联系管理员！"));
		return;
	}
	if (m_level <= 1)
	{
		QMessageBox::warning(this, "错误警告", QString("你当前没有权限添加用户，请登录！"));
		return;
	}
	QUserAddNew dlg;
	dlg.setTitle("添加新用户");
	UserInfo info;
	info.mLevel = m_level;
	info.strName = m_strName;
	dlg.setCurrentUser(info);
	QStringList strList = m_pUserDB->Users(-1);
	dlg.SetFilterList(strList);
	if (dlg.exec() == QDialog::Accepted)
	{
		UserInfo m_userInfo = dlg.getUserInfo();
		if (m_pUserDB->Replace(m_userInfo.strName, m_userInfo.passwd, m_userInfo.mLevel))
		{
			ui.label_Status->setText(QString("%1 添加完成！！！").arg(m_userInfo.strName));
			OnUpdateUserList();
		}
	}
}

void QUserManagerDlg::onDelUser()
{
	int row = ui.tableView->currentIndex().row();
	QAbstractItemModel *model = ui.tableView->model();
	QModelIndex index = model->index(row, 0);//选中行第一列的内容
	QVariant data = model->data(index);
	QString strName = data.toString();
	if (m_strName.isEmpty())
	{
		QMessageBox::warning(this, "错误警告", QString("你还没登录，请登录或请联系管理员！"));
		return;
	}
	if (strName.isEmpty())
	{
		QMessageBox::warning(this, "错误警告", QString("请选中你要删除的用户！"));
		return;
	}
	if (strName == "root")
	{
		QMessageBox::warning(this, "错误警告", QString("你当前没有权限删除root用户，请联系超超级管理员！"));
		return;
	}
	if (strName == m_strName)
	{
		QMessageBox::warning(this, "错误警告", QString("你当前没有权限删除自己的账户，请联系管理员！"));
		return;
	}
	if (QMessageBox::Ok ==QMessageBox::warning(this, "错误警告", QString("确定删除用户: %1 ?").arg(strName), QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel))
 	{
 		if (m_pUserDB->Delete(strName))
 		{
			ui.label_Status->setText(QString("%1 已删除！").arg(strName));
 			OnUpdateUserList();
 		}
 	}
}

void QUserManagerDlg::onModifyUser()
{
	/*设置选择用户的信息*/
	int row = ui.tableView->currentIndex().row();
	QAbstractItemModel *model = ui.tableView->model();
	QModelIndex index = model->index(row, 0);//选中行第一列的内容
	QVariant data = model->data(index);
	QString strName = data.toString();
	if (m_strName.isEmpty())
	{
		QMessageBox::warning(this, "错误警告", QString("你还没登录，请登录或请联系管理员！"));
		return;
	}
	if (strName.isEmpty())
	{
		QMessageBox::warning(this, "错误警告", QString("你没有选中需要修改的用户，请联系管理员！"));
		return;
	}
 	QUserAddNew dlg;
	dlg.setTitle("修改用户信息");
	UserInfo info;
	info.mLevel = m_level;
	info.strName = m_strName;
	dlg.setCurrentUser(info);
	UserInfo mUserInfo = mMapUserInfos[strName];
	dlg.setUserInfo(mUserInfo);
	if (dlg.exec() == QDialog::Accepted)
	{
		UserInfo m_userInfo = dlg.getUserInfo();
		if (m_pUserDB->Replace(m_userInfo.strName, m_userInfo.passwd, m_userInfo.mLevel))
		{
			ui.label_Status->setText(QString("%1 修改完成！！！").arg(m_userInfo.strName));
			OnUpdateUserList();
		}
	}
}

void QUserManagerDlg::OnUpdateUserList()
{
	mMapUserInfos.clear();
	map_userInfos v = m_pUserDB->getAllUserInfos();
	int i = 0;
	plistModel->clear();
	QStringList headerList;
	headerList << "用户名" << "职位"<<"权限等级";
	plistModel->setHorizontalHeaderLabels(headerList);
	
	for each(UserInfo item in v)
	{
		if (item.strName.isEmpty())
			continue;
		if (item.mLevel > m_level)
			continue;
		plistModel->setItem(i, 0, new QStandardItem(item.strName));
		//设置字符颜色 
 		//plistModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
 		//设置字符位置 
 		plistModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		plistModel->setItem(i, 1, new QStandardItem(item.strAbout));
		plistModel->setItem(i, 2, new QStandardItem(QString("%1").arg(sc_level[item.mLevel])));
		i++;
		mMapUserInfos.insert(item.strName, item);
	}
}
