#ifndef  _QLOGINDLG_H_
#define  _QLOGINDLG_H_

#include <QDialog>
#include "ui_QLoginDlg.h"
#include "QUserDB.h"
//µÇÂ¼´°¿Ú
class QLoginDlg : public QDialog
{
	Q_OBJECT

public:
	QLoginDlg(QUserDB* pUserDB);
	~QLoginDlg();
	
	void AddItems2List(QStringList strList);
	void setCurrentUser(QString str);
private:
	Q_SLOT void OnEditTextChanged(const QString &text);
	Q_SLOT void OnClicked();
private:
	Ui::QLoginDlg ui;
	QComboBox* _UserNameComboBox{ nullptr };
	QLineEdit* _PasswdLineEdit{ nullptr };

	QUserDB* _pUserDB;
signals:
	void sgLogin(QString userName, int nLevel);
};

#endif // _QLOGINDLG_H_