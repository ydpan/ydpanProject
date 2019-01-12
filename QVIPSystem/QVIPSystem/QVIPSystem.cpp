#include "QVIPSystem.h"
#include "QVipMemberInfo.h"
QVIPSystem::QVIPSystem(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pDb = new QSqliteVipSystemDB(_DBFILE_NAME_);
	m_pDb->InitDatabase();
	connect(ui.AddVIPMember_action, SIGNAL(triggered()), this, SLOT(onAction()));
}

Q_SLOT void QVIPSystem::onAction()
{
	QString strObj = sender()->objectName();
	if ("AddVIPMember_action" == strObj) {
		QVipMemberInfo dlg;
		int vid = m_pDb->getVipMemberMaxID();
		dlg.setBestID(vid);
		if (dlg.exec() == QDialog::Accepted)
		{
			tag_VipMemberInfo m_newMember = dlg.getNewInfo();
			m_pDb->AddNewVipMemberInfo(m_newMember);
		}
	}
}
