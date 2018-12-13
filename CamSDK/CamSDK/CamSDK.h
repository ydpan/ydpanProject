#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CamSDK.h"
#include "udpserver.h"
using namespace  boost;

class CamSDK : public QMainWindow
{
	Q_OBJECT

public:
	CamSDK(QWidget *parent = Q_NULLPTR);
	Q_SLOT void onFindDevice();
	void handMsgCallBack(tagUdpData tagData);
private:
	Ui::CamSDKClass ui;

	boost::shared_ptr<UDPService> m_pUdpServer;
	boost::asio::ip::udp::endpoint _fromPoint;

	int m_nMsgNr{ 0 };
};
