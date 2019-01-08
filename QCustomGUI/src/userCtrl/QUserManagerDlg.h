#ifndef  _QUSERMANAGERDLG_H_
#define  _QUSERMANAGERDLG_H_

#include <QDialog>
#include "ui_QUserManagerDlg.h"
#include "QUserDB.h"
#include <QStandardItemModel>
#include <QSharedPointer>
class QUserManagerDlg : public QDialog
{
	Q_OBJECT

public:
	QUserManagerDlg(QUserDB* pUserDB);
	~QUserManagerDlg();
	void SetUserAndLevel(QString strName, int level);
	void OnUpdateUserList();
private slots:
	void onAddUser();
	void onDelUser();
	void onModifyUser();
private:
	QString m_strName;
	int m_level{ 0 };
	Ui::QUserManagerDlg ui;
	QUserDB* m_pUserDB{ nullptr };
	QSharedPointer<QStandardItemModel> plistModel;
	QMap<QString, UserInfo> mMapUserInfos;
};
#endif // !QUSERMANAGERDLG_H_
