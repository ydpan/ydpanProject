#ifndef _QVIPSYSTEM_H_
#define _QVIPSYSTEM_H_

#include <QtWidgets/QMainWindow>
#include "ui_QVIPSystem.h"

#include "QSqliteVipSystemDB.h"
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
	Q_SLOT void onAction();
private:
	Ui::QVIPSystemClass ui;

	QSqliteVipSystemDB *m_pDb;
};
#endif
