#include "QUserManagerDlg.h"
#include <QMessageBox>

static const QMap<int, QString> sc_level = {
	{ USER_LEVEL_ADM, QString::fromLocal8Bit("管理员") },
	{ USER_LEVEL_OPT, QString::fromLocal8Bit("操作员") },
	{ USER_LEVEL_ADM, QString::fromLocal8Bit("根管理员") }
};


QString levelName(int nLevel) {
	QString strLevel = QString::fromLocal8Bit("无用户");
	if (sc_level.contains(nLevel))
		strLevel = sc_level[nLevel];
	return strLevel;
}

QUserManagerDlg::QUserManagerDlg(QUserDB* pLogic)
	: QDialog(nullptr)
	, _pUserDB(pLogic)
{
	ui.setupUi(this);
	ui.tableView;
	QStringList headerList;
	headerList << "userName" << "level";
	plistModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel);
	//plistModel->setColumnCount(headerList.size());
	plistModel->setHorizontalHeaderLabels(headerList);
	ui.tableView->setModel(plistModel.data());

	map_userInfos v =  pLogic->getAllUserInfos();
	int i = 0;
	for each(ST_UserInfo item in v)
	{
		plistModel->setItem(i, 0, new QStandardItem(item.strName));
		//设置字符颜色 
		plistModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//设置字符位置 
		plistModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		plistModel->setItem(i, 1, new QStandardItem(QString("%1").arg(item.nlevel)));
	}


	connect(ui.tp_listwidget_users, &QListWidget::itemDoubleClicked, this, &QUserManagerDlg::OnDbClickToDel);

	QStringList lstUser = _pUserDB->Users(-1);
	ui.tp_listwidget_users->addItems(lstUser);
}

QUserManagerDlg::~QUserManagerDlg()
{
}

void QUserManagerDlg::on_tp_button_add_user_clicked()
{	
	if (_pUserDB->CurUser() != "root")
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("仅有根用户有此权限！"));
		return;
	}

	QString user = ui.tp_edit_new_user->text();

	QStringList users = _pUserDB->Users(-1);
	if (users.contains(user))
	{
		return;
	}

	int nLevel = USER_LEVEL_NOUSER;	

	QVariant vLevel = ui.tp_cmb_add_user_level->currentData();
	if (!vLevel.isNull())
	{
		nLevel = vLevel.toInt();
	}

	if (_pUserDB->Replace(user, "123456", nLevel))
	{
		ui.tp_listwidget_users->addItem(user);
	}
	
	OnUpdateUserList();
}

void QUserManagerDlg::OnDbClickToDel(QListWidgetItem * item)
{	
	QString str = item->text();
	if (str == "root")
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("不能删除根用户！"));
		return;
	}
	if (_pUserDB->CurUser() != "root")
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("仅有根用户有此权限！"));
		return;
	}

	if (QMessageBox::Ok ==
		QMessageBox::warning(this
			, "Warning"
			, QString::fromLocal8Bit("确定删除用户: %1?").arg(str)
			, QMessageBox::Cancel | QMessageBox::Ok
			, QMessageBox::Cancel))
	{
		if (_pUserDB->Delete(str))
		{
			ui.tp_listwidget_users->removeItemWidget(item);
			delete item;
			
			//OnUpdateUserList();
		}
	}
}


void QUserManagerDlg::OnUpdateUserList()
{
	for (int i = 0; i < 3; i++)
	{
		plistModel->setItem(i, 0, new QStandardItem("2009441676"));
		//设置字符颜色 
		plistModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
		//设置字符位置 
		plistModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);
		plistModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("哈哈")));
	}
}
