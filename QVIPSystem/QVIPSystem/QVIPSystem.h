#ifndef _QVIPSYSTEM_H_
#define _QVIPSYSTEM_H_

#include <QtWidgets/QMainWindow>
#include "ui_QVIPSystem.h"

#include "QSqliteVipSystemDB.h"
#include <QStandardItemModel>
/*
需求：
1、人员信息录取
2、人员销售金额录取
3、人员晒单次数录取
4、记录每天的人员晒单和销售金额
5、根据金额、晒单统计计算积分
*/
class QVIPSystem : public QMainWindow
{
	Q_OBJECT

public:
	QVIPSystem(QWidget *parent = Q_NULLPTR);
	~QVIPSystem();
	Q_SLOT void onAction();
	Q_SLOT void onShowUserInfo();
	Q_SLOT void onShowRecord();
	Q_SLOT void onModifyUserInfo();
	Q_SLOT void onDelUserInfo();
	Q_SLOT void onAddNewUserInfo();
	Q_SLOT void onModify();
	Q_SLOT void onTableViewClick(QModelIndex);
	Q_SLOT void onTableViewRecordClick(QModelIndex);
	Q_SLOT void onAddRecord();
private:
	Ui::QVIPSystemClass ui;

	QSqliteVipSystemDB *m_pDb{NULL};
	QStandardItemModel *m_pModel{ NULL };
	QStandardItemModel *m_pRecord{ NULL };
	QMap<int, VipMemberInfo> mMap;
	int currentID{0};
};
#endif
