#include "boostSeriport.h"

boostSeriport::boostSeriport(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitSerialDevice();
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onSendData()));
	connect(this, SIGNAL(sgRecvData(QVariantMap)), this, SLOT(onRecvData(QVariantMap)));
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
	m_timer.start(5000);
}

boostSeriport::~boostSeriport()
{
	if(m_pSerialDevice)
		m_pSerialDevice->stopThread();
}

void boostSeriport::InitSerialDevice()
{
	m_serialParams.baudRate = 115200;
	m_serialParams.serialPort = "COM4";
	m_serialParams.flowControl = 0;
	m_serialParams.parity = 0;
	m_serialParams.stopBits = 0;

	m_pSerialDevice = boost::make_shared<SerialPortDevice>();
	m_pSerialDevice->setSerialParams(m_serialParams);
	m_pSerialDevice->setTimeOut(m_serialPortTimeOut);
	m_pSerialDevice->setCallbackFunc(boost::bind(&boostSeriport::serialCallback,this,_1));
	m_pSerialDevice->startThread();
}

void boostSeriport::DataGram_Callback(const DataGramPtr &msg)
{
	m_pSerialDevice->writeDataGram(*msg);
}

void boostSeriport::serialCallback(DataGramPtr pDatagram)
{
	QVariantMap mapData;
	mapData["sender"] = pDatagram->sender;
	mapData["recvder"] = pDatagram->receiver;
	QByteArray mdata;
	for (int nIndex = 0; nIndex < pDatagram->data.size(); nIndex++) {
		mdata.append(pDatagram->data.at(nIndex));
	}
	mapData["data"] = mdata;
	emit(sgRecvData(mapData));
}

Q_SLOT void boostSeriport::onTimeOut()
{
	DataGramPtr p = boost::make_shared<DataGram>();
	p->data.push_back(1);
	p->data.push_back(2);
	p->sender = 12;
	p->receiver = 34;
	DataGram_Callback(p);
}

Q_SLOT void boostSeriport::onRecvData(QVariantMap mapdata)
{
	QString strRecvSer = QString("%1").arg(mapdata.value("sender").toString());
	QString strRecvRer = QString("%1").arg(mapdata.value("recvder").toString());
	QByteArray mdata = mapdata.value("data").toByteArray();
	ui.lineEdit_revsender->setText(strRecvSer);
	ui.lineEdit_recvrer->setText(strRecvRer);
	
	QString strData;
	for (int nIndex = 0; nIndex < mdata.size(); nIndex++) {
		strData.append(QString("%1").arg(mdata.at(nIndex)));
	}
	ui.lineEdit_revdata->setText(strData);
}

Q_SLOT void boostSeriport::onSendData()
{
	QString strsender = ui.lineEdit_sender->text();
	QString strrecvder = ui.lineEdit_receiver->text();
	QString strdata = ui.lineEdit_senddata->text();

	DataGramPtr p = boost::make_shared<DataGram>();
	strdata.remove(" ");
	for (int nIndex = 0; nIndex < strdata.size(); nIndex++) {
		p->data.push_back(strdata.at(nIndex).toLatin1());
	}
	
	p->sender = strsender.toInt();
	p->receiver = strrecvder.toInt();
	DataGram_Callback(p);

}
