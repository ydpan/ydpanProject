#include "CamSDK.h"

CamSDK::CamSDK(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

	connect(ui.m_FindDevice, SIGNAL(clicked()), this, SLOT(onFindDevice()));
	m_pUdpServer = boost::make_shared<UDPService>("127.0.0.1",1111);
	m_pUdpServer->setCallbackFunc(boost::bind(&CamSDK::handMsgCallBack, this, _1));
	m_pUdpServer->startThread();
}

Q_SLOT void CamSDK::onFindDevice()
{
	boost::array<uint8_t, 8> buff = { { 0x42, 0x01, 0x00, 0x02, 0x00, 0x00 } };
	((uint16_t*)&buff)[3] = htons(m_nMsgNr++);

	
	tagUdpData m_writeData;
	boost::asio::ip::address_v4::broadcast();
	boost::asio::ip::udp::endpoint endpointa(boost::asio::ip::address::from_string("127.0.0.1"), 3956);
	m_writeData.fromPoint = endpointa;// boost::asio::ip::udp::endpoint(add, 3956);
	m_writeData._byteData = boost::shared_ptr<ByteVector>(new ByteVector);
	m_writeData._byteData->resize(8, 0);
	string ss =  m_writeData.fromPoint.address().to_string();
	unsigned char *pData = m_writeData._byteData->data();
	memcpy(pData, buff.data(), 8);

	m_pUdpServer->writeRaw(m_writeData);
}

void CamSDK::handMsgCallBack(tagUdpData tagData)
{
    string strAddr = tagData.fromPoint.address().to_string();
	string port = to_string(tagData.fromPoint.port());
	int a = 0;
}