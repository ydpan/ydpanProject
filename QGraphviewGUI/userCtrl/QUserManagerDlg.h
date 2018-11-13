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
private slots:
	void on_tp_button_add_user_clicked();
	void OnDbClickToDel(QListWidgetItem * item);
	void OnUpdateUserList();
private:

private:
	Ui::QUserManagerDlg ui;
	QUserDB* _pUserDB{ nullptr };
	QSharedPointer<QStandardItemModel> plistModel;
};
#endif // !QUSERMANAGERDLG_H_
