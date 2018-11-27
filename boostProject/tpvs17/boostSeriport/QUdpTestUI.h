#ifndef  _QUDPTESTUI_H_
#define _QUDPTESTUI_H_


#include <QWidget>
#include "ui_QUdpTestUI.h"
#include "udpserver.h"
#include <boost/shared_ptr.hpp>
#include "GVCPDevice.h"
class QUdpTestUI : public QWidget
{
	Q_OBJECT

public:
	QUdpTestUI(QWidget *parent = Q_NULLPTR);
	~QUdpTestUI();
	void InitUDP();
private:
	Ui::QUdpTestUI ui;
	boost::shared_ptr<UDPService> m_pUdpServer;
	boost::shared_ptr<GVCPDevice> m_pGVSPDevice;
};

#endif // ifndef QUDPTESTUI_H_