#include "QUdpTestUI.h"

QUdpTestUI::QUdpTestUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUDP();
}

QUdpTestUI::~QUdpTestUI()
{
}

void QUdpTestUI::InitUDP()
{

// 	m_pUdpServer = boost::make_shared<UDPService>(3956);
// 	m_pUdpServer->startThread();

	m_pGVSPDevice = boost::make_shared<GVCPDevice>();
	m_pGVSPDevice->InitDevice();
}
