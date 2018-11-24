#ifndef _BOOSTSERIALPORT_H_
#define _BOOSTSERIALPORT_H_

#include <QtWidgets/QMainWindow>
#include "ui_boostSeriport.h"
#include "SerialPortDevice.h"
#include <QTimer>
#include <QMap>
#include <QVariant>

class boostSeriport : public QMainWindow
{
	Q_OBJECT

public:
	boostSeriport(QWidget *parent = Q_NULLPTR);
	virtual ~boostSeriport();
	void InitSerialDevice();

	void DataGram_Callback(const DataGramPtr &msg);
	//void XM_RawData_Callback(const xm_msgs::xm_Uint8Array::ConstPtr &msg);

	void serialCallback(DataGramPtr pDatagram);
	Q_SLOT void onTimeOut();
	Q_SLOT void onRecvData(QVariantMap mapdata);
	Q_SLOT void onSendData();
signals:
	void sgRecvData(QVariantMap recvData);
private:
	Ui::boostSeriportClass ui;

	SerialPortParams m_serialParams;
	int m_serialPortTimeOut{100};
	boost::shared_ptr<SerialPortDevice> m_pSerialDevice;

	QTimer m_timer;
};

#endif
