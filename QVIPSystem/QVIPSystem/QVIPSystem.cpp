#include "QVIPSystem.h"
#include "QVipMemberInfo.h"
#include "QConsumeDlg.h"
#pragma execution_character_set("utf-8")

QVIPSystem::QVIPSystem(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pDb = new QSqliteVipSystemDB(_DBFILE_NAME_);
	m_pDb->InitDatabase();
	//connect(ui.AddVIPMember_action, SIGNAL(triggered()), this, SLOT(onAction()));
	connect(ui.m_pbAdd, SIGNAL(clicked()), this, SLOT(onAddNewUserInfo()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onShowUserInfo()));
	connect(ui.m_pbModify, SIGNAL(clicked()), this, SLOT(onModifyUserInfo()));
	connect(ui.m_pbDel, SIGNAL(clicked()), this, SLOT(onDelUserInfo()));
	connect(ui.m_pbMod, SIGNAL(clicked()), this, SLOT(onModify()));
	m_pModel = new QStandardItemModel;
	ui.tableView->setModel(m_pModel);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	//ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableView->horizontalHeader()->setMaximumHeight(100);
	connect(ui.tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(onTableViewClick(QModelIndex)));
	onShowUserInfo();


	m_pRecord = new QStandardItemModel;
	ui.tableView_2->setModel(m_pRecord);
	ui.tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(ui.tableView_2, SIGNAL(clicked(QModelIndex)), this, SLOT(onTableViewRecordClick(QModelIndex)));

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onAddRecord()));
}

QVIPSystem::~QVIPSystem()
{
	if (m_pDb) {
		m_pDb->FreeDatabase();
		delete m_pDb;
		m_pDb = NULL;
	}
}

Q_SLOT void QVIPSystem::onAction()
{
// 	QString strObj = sender()->objectName();
// 	if ("AddVIPMember_action" == strObj) {
// 		QVipMemberInfo dlg;
// 		int vid = m_pDb->getVipMemberMaxID();
// 		dlg.setBestID(vid);
// 		if (dlg.exec() == QDialog::Accepted)
// 		{
// 			tag_VipMemberInfo m_newMember = dlg.getNewInfo();
// 			m_pDb->AddNewVipMemberInfo(m_newMember);
// 		}
// 		onShowUserInfo();
// 	}
}

Q_SLOT void QVIPSystem::onShowUserInfo()
{
	m_pModel->clear();
	m_pModel->setHorizontalHeaderItem(0, new QStandardItem(QString("VIP编号")));
	m_pModel->setHorizontalHeaderItem(1, new QStandardItem(QString("姓名")));
	m_pModel->setHorizontalHeaderItem(2, new QStandardItem(QString("积分")));
	m_pModel->setHorizontalHeaderItem(3, new QStandardItem(QString("消费总金额(元)")));
	m_pModel->setHorizontalHeaderItem(4, new QStandardItem(QString("晒单次数")));
	m_pModel->setHorizontalHeaderItem(5, new QStandardItem(QString("电话")));
	m_pModel->setHorizontalHeaderItem(6, new QStandardItem(QString("地址")));
	m_pModel->setHorizontalHeaderItem(7, new QStandardItem(QString("加入时间")));
	m_pModel->setHorizontalHeaderItem(8, new QStandardItem(QString("更新时间")));
	//m_pModel->setHorizontalHeaderItem(4, new QStandardItem(QString("VIP编号")));
	
	mMap.clear();
	m_pDb->GetAllVipMemberInfos(mMap);
	int nIndex = 0;
	for(QMap<int,VipMemberInfo>::iterator its = mMap.begin();its!=mMap.end();++its)
	{
		m_pModel->setItem(nIndex, 0, new QStandardItem(QString("%1").arg(its->nVipNum)));
		m_pModel->setItem(nIndex, 1, new QStandardItem(QString("%1").arg(its->strName)));
		m_pModel->setItem(nIndex, 3, new QStandardItem(QString("%1").arg(its->nMonetary)));
		m_pModel->setItem(nIndex, 2, new QStandardItem(QString("%1").arg(its->nIntegration)));
		m_pModel->setItem(nIndex, 4, new QStandardItem(QString("%1").arg(its->nDrying_list)));
		m_pModel->setItem(nIndex, 5, new QStandardItem(QString("%1").arg(its->nPhoneNumber)));
		m_pModel->setItem(nIndex, 6, new QStandardItem(QString("%1").arg(its->strAddress)));
		m_pModel->setItem(nIndex, 7, new QStandardItem(QString("%1").arg(its->strTime)));
		m_pModel->setItem(nIndex, 8, new QStandardItem(QString("%1").arg(its->strUpdateTime)));
		nIndex++;
	}
}

Q_SLOT void QVIPSystem::onShowRecord()
{
	m_pRecord->clear();
	m_pRecord->setHorizontalHeaderItem(0, new QStandardItem(QString("VIP编号")));
	m_pRecord->setHorizontalHeaderItem(1, new QStandardItem(QString("姓名")));
	m_pRecord->setHorizontalHeaderItem(2, new QStandardItem(QString("消费总金额(元)")));
	m_pRecord->setHorizontalHeaderItem(3, new QStandardItem(QString("晒单次数")));
	m_pRecord->setHorizontalHeaderItem(4, new QStandardItem(QString("时间")));

	QMap<int, MonetaryRecord> mRecordMap;
	m_pDb->GetRecords(mRecordMap,"");
	int nIndex = 0;
	for (QMap<int, MonetaryRecord>::iterator its = mRecordMap.begin(); its != mRecordMap.end(); ++its)
	{
		m_pRecord->setItem(nIndex, 0, new QStandardItem(QString("%1").arg(its->nVipNum)));
		m_pRecord->setItem(nIndex, 1, new QStandardItem(QString("%1").arg(its->strName)));
		m_pRecord->setItem(nIndex, 2, new QStandardItem(QString("%1").arg(its->nDailyMoney)));
		m_pRecord->setItem(nIndex, 3, new QStandardItem(QString("%1").arg(its->nDryListCount)));
		m_pRecord->setItem(nIndex, 4, new QStandardItem(QString("%1").arg(its->strTime)));
		nIndex++;
	}
}

Q_SLOT void QVIPSystem::onModifyUserInfo()
{
	if (currentID != 0)
	{
		QVipMemberInfo dlg;
		VipMemberInfo info = mMap.value(currentID);
		dlg.setInfo(info);
		if (dlg.exec() == QDialog::Accepted)
		{
			tag_VipMemberInfo m_newMember = dlg.getNewInfo();
			m_pDb->ModifyVipMemberInfo(m_newMember);
		}
		currentID = 0;
		onShowUserInfo();
	}

}

Q_SLOT void QVIPSystem::onDelUserInfo()
{
	if (currentID != 0) {
		VipMemberInfo info = mMap.value(currentID);
		m_pDb->DelVipMemberInfo(info);
		currentID = 0;
		onShowUserInfo();
	}
}

Q_SLOT void QVIPSystem::onAddNewUserInfo()
{
	QVipMemberInfo dlg;
	int vid = m_pDb->getVipMemberMaxID();
	dlg.setBestID(vid);
	if (dlg.exec() == QDialog::Accepted)
	{
		tag_VipMemberInfo m_newMember = dlg.getNewInfo();
		m_pDb->AddNewVipMemberInfo(m_newMember);
	}
	onShowUserInfo();
}

Q_SLOT void QVIPSystem::onModify()
{
	if (currentID != 0)
	{
		QVipMemberInfo dlg;
		VipMemberInfo info = mMap.value(currentID);
		dlg.setInfo(info);
		dlg.setModifyMoney();
		if (dlg.exec() == QDialog::Accepted)
		{
			tag_VipMemberInfo m_newMember = dlg.getNewInfo();
			m_pDb->ModifyVipMemberInfo(m_newMember);
		}
		currentID = 0;
		onShowUserInfo();
	}
}

Q_SLOT void QVIPSystem::onTableViewClick(QModelIndex mIndex)
{
	QModelIndex ItemIndex = m_pModel->index(mIndex.row(), 0);
	currentID = ItemIndex.data().toInt();
	/*

	*/
}

Q_SLOT void QVIPSystem::onTableViewRecordClick(QModelIndex mIndex)
{
	QModelIndex ItemIndex = m_pModel->index(mIndex.row(), 0);
	currentID = ItemIndex.data().toInt();
}

Q_SLOT void QVIPSystem::onAddRecord()
{
	QConsumeDlg dlg;
	QMap<QString, int> mList;
	m_pDb->GetAllNames(mList);
	dlg.setMap(mList);
	if (dlg.exec() == QDialog::Accepted)
	{
		tag_MonetaryRecord mRecord = dlg.getMonetaryRecord();
		m_pDb->AddNewConsumeInfo(mRecord);

		//计算积分
		QString strName = mRecord.strName;
		int vipNum = mRecord.nVipNum;

		VipMemberInfo info = mMap.value(vipNum);
		info.nMonetary += mRecord.nDailyMoney;
		info.nDrying_list += mRecord.nDryListCount;
		info.nIntegration = info.nMonetary / 50.0+info.nDrying_list;
		m_pDb->ModifyVipMemberInfo(info);

	}
	onShowRecord();
	onShowUserInfo();
}
