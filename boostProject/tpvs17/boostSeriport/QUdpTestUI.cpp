#include "QUdpTestUI.h"
#include "DeviceInfo.h"

QUdpTestUI::QUdpTestUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUDP();

// 	DeviceInfo m_device;
// 	m_device.Init();
// 
// 	uint aaa = m_device.GetRegisterData(0xd00);
// 	uint data = 1000;
// 	m_device.SetRegisterData(0xd00, data);
// 	int aa =m_device.GetRegisterData(0xd00);
// 	std::string strSubNetMask = "192.168.8.123";
// 	unsigned int nCurrentSubNetMask = inet_addr(strSubNetMask.c_str());
// 	m_device.SetRegisterData(0xd18, nCurrentSubNetMask);
// 	unsigned int nc = m_device.GetRegisterData(0xd18);
// 	int a= 0;
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
