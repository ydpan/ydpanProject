#include "CamSDK.h"

CamSDK::CamSDK(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

	connect(ui.m_FindDevice, SIGNAL(clicked()), this, SLOT(onFindDevice()));
}

Q_SLOT void CamSDK::onFindDevice()
{

}
