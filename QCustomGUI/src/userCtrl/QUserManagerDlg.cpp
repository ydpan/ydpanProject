#include "QUserManagerDlg.h"
#include <QMessageBox>
#include "QUserAddNew.h"
#include "UserLoginDefine.h"
#pragma execution_character_set("utf-8")



QString levelName(int nLevel) {
	QString strLevel = QString("���û�");
	if (sc_level.contains(nLevel))
		strLevel = sc_level[nLevel];
	return strLevel;
}

QUserManagerDlg::QUserManagerDlg(QUserDB* pLogic)
	: QDialog(nullptr)
	, m_pUserDB(pLogic)
{
	ui.setupUi(this);
	QStringList headerList;
	headerList << "�û���" << "�ȼ�";
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
	ui.label_Status->setText("");
}


void QUserManagerDlg::onAddUser()
{
	if (m_level <= 1)
	{
		QMessageBox::warning(this, "Warning", QString("�㵱ǰû��Ȩ������û������¼��"));
		return;
	}
	QUserAddNew dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		UserInfo m_userInfo = dlg.getUserInfo();
		if (m_pUserDB->Replace(m_userInfo.strName, m_userInfo.passwd, m_userInfo.mLevel))
		{
			ui.label_Status->setText(QString("%1 �����ɣ�����").arg(m_userInfo.strName));
			OnUpdateUserList();
		}

	}
}

void QUserManagerDlg::onDelUser()
{
	int row = ui.tableView->currentIndex().row();
	QAbstractItemModel *model = ui.tableView->model();
	QModelIndex index = model->index(row, 0);//ѡ���е�һ�е�����
	QVariant data = model->data(index);
	QString strName = data.toString();
	if (strName.isEmpty())
	{
		QMessageBox::warning(this, "Warning", QString("��ѡ����Ҫɾ�����û���"));
		return;
	}
	if (strName == "root")
	{
		QMessageBox::warning(this, "Warning", QString("�㵱ǰû��Ȩ��ɾ��root�û�������ϵ����������Ա��"));
		return;
	}

	if (QMessageBox::Ok ==QMessageBox::warning(this, "Warning", QString("ȷ��ɾ���û�: %1?").arg(strName), QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel))
 	{
 		if (m_pUserDB->Delete(strName))
 		{
			ui.label_Status->setText(QString("%1 ��ɾ����").arg(strName));
 			OnUpdateUserList();
 		}
 	}
}

void QUserManagerDlg::onModifyUser()
{
	/*����ѡ���û�����Ϣ*/
	int row = ui.tableView->currentIndex().row();
	QAbstractItemModel *model = ui.tableView->model();
	QModelIndex index = model->index(row, 0);//ѡ���е�һ�е�����
	QVariant data = model->data(index);
	QString strName = data.toString();

 	QUserAddNew dlg;
	UserInfo mUserInfo = mMapUserInfos[strName];
	dlg.setUserInfo(mUserInfo);
	if (dlg.exec() == QDialog::Accepted)
	{
		UserInfo m_userInfo = dlg.getUserInfo();
		if (m_pUserDB->Replace(m_userInfo.strName, m_userInfo.passwd, m_userInfo.mLevel))
		{
			ui.label_Status->setText(QString("%1 �޸���ɣ�����").arg(m_userInfo.strName));
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
	headerList << "�û���" << "ְλ"<<"Ȩ�޵ȼ�";
	plistModel->setHorizontalHeaderLabels(headerList);
	
	for each(UserInfo item in v)
	{
		if (item.strName.isEmpty())
			continue;
		if (item.mLevel > m_level)
			continue;
		plistModel->setItem(i, 0, new QStandardItem(item.strName));
		//�����ַ���ɫ 
 		//plistModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
 		//�����ַ�λ�� 
 		plistModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		plistModel->setItem(i, 1, new QStandardItem(item.strAbout));
		plistModel->setItem(i, 2, new QStandardItem(QString("%1").arg(sc_level[item.mLevel])));
		i++;
		mMapUserInfos.insert(item.strName, item);
	}
}
