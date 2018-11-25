#include "boostSeriport.h"

boostSeriport::boostSeriport(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitSerialDevice();
	{
		Expect_Global_Speed.X = 0;
		Expect_Global_Speed.Y = 0;
		Expect_Global_Speed.Z = 0;

		Expect_Robot_Speed.X = 0;
		Expect_Robot_Speed.Y = 0;
		Expect_Robot_Speed.Z = 0;

		Expect_Motor_Speed.M1 = 0;
		Expect_Motor_Speed.M2 = 0;
		Expect_Motor_Speed.M3 = 0;

		Mesure_Global_Coordinate.X = 0;
		Mesure_Global_Coordinate.Y = 0;
		Mesure_Global_Coordinate.Z = 0;

		Mesure_Robot_Coordinate.X = 0;
		Mesure_Robot_Coordinate.Y = 0;
		Mesure_Robot_Coordinate.Z = 0;

		Mesure_Global_Speed.X = 0;
		Mesure_Global_Speed.Y = 0;
		Mesure_Global_Speed.Z = 0;

		Mesure_Robot_Speed.X = 0;
		Mesure_Robot_Speed.Y = 0;
		Mesure_Robot_Speed.Z = 0;

		Mesure_Motor_Speed.M1 = 0;
		Mesure_Motor_Speed.M2 = 0;
		Mesure_Motor_Speed.M3 = 0;

		Mesure_Motor_Distance.M1 = 0;
		Mesure_Motor_Distance.M2 = 0;
		Mesure_Motor_Distance.M3 = 0;


		Mesure_odom_filter.X = 0;
		Mesure_odom_filter.Y = 0;
		Mesure_odom_filter.Z = 0;
	}
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onSendData()));
	connect(ui.boost_startbtn, SIGNAL(clicked()), this, SLOT(onStart()));
	connect(ui.boost_speedbtn, SIGNAL(clicked()), this, SLOT(onSpeed()));
	connect(ui.boost_subbtn, SIGNAL(clicked()), this, SLOT(onSub()));
	connect(ui.s_btn, SIGNAL(clicked()), this, SLOT(onCtrl()));
	connect(ui.w_btn, SIGNAL(clicked()), this, SLOT(onCtrl()));
	connect(ui.a_btn, SIGNAL(clicked()), this, SLOT(onCtrl()));
	connect(ui.d_btn, SIGNAL(clicked()), this, SLOT(onCtrl()));

	connect(this, SIGNAL(sgRecvData(QVariantMap)), this, SLOT(onRecvData(QVariantMap)));
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
	m_timer.start(200);
	startWheel();
}


boostSeriport::~boostSeriport()
{
	if(m_pSerialDevice)
		m_pSerialDevice->stopThread();
}

void boostSeriport::InitSerialDevice()
{
	m_serialParams.baudRate = 115200;
	m_serialParams.serialPort = "COM10";
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

	const uint8_t *pData = pDatagram->data.data();
	_command_state = (COMMAND)*pData;

	if (_command_state == Shaking_Hands) {
		pub_Float_Vector3(Shaking_Hands, 1, 1, 1);
		return;
	}
	if (_command_state == Set_Global_Speed)
		return;
	if (_command_state == Set_Robot_Speed)
		return;
	if (_command_state == Read_Global_Coordinate)
	{
		/****x,y Difference Filter****/
		float x = *(float *)(pData + 1);
		float delta_x = x - Mesure_Global_Coordinate.X;
		//arm architecture meets problem: bus error
		if (delta_x < 0.5  &&  delta_x > -0.5) {
			Mesure_odom_filter.X += delta_x;
		}
		Mesure_Global_Coordinate.X = *(float *)(pData + 1);

		float y = *(float *)(pData + 1 + 4);
		float delta_y = y - Mesure_Global_Coordinate.Y;
		if (delta_y < 0.5  &&  delta_y > -0.5) {
			Mesure_odom_filter.Y += delta_y;
		}
		Mesure_Global_Coordinate.Y = *(float *)(pData + 1 + 4);

		/****the z can not be filtered
		 * or the coordinates  system will be error when you reset the Embedded****/
		 /********
		 float z = *(float *)(pData + 1 + 4 + 4) - Mesure_Global_Coordinate.Z;
		 if( z < 0.5  &&  z > -0.5 ){
			 Mesure_odom_filter.Z  += z;
		 }
		 Mesure_Global_Coordinate.Z =  *(float *)(pData + 1 + 4 + 4);
		 ********/
		float z = *(float *)(pData + 1 + 4 + 4);
		float delta_z = z - Mesure_odom_filter.Z;
		Mesure_odom_filter.Z = z;

		return;
	}
	if (_command_state == Set_Motor_Speed)
		return;
	if (_command_state == Read_Motor_Speed)
		return;
	if (_command_state == Read_Motor_Mileage)
		return;
	if (_command_state == Clear_Coordinate_Data)
		return;



}

Q_SLOT void boostSeriport::onTimeOut()
{//心跳
// 	DataGramPtr p = boost::make_shared<DataGram>();
// 	p->data.push_back(1);
// 	p->data.push_back(2);
// 	p->sender = 12;
// 	p->receiver = 34;
// 	DataGram_Callback(p);
	pub_Command(Read_Global_Coordinate);
	pub_Float_Vector3(Set_Robot_Speed, Expect_Robot_Speed.X, Expect_Robot_Speed.Y, Expect_Robot_Speed.Z);
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

Q_SLOT void boostSeriport::onStart()
{
	Expect_Robot_Speed.X = 0;
	Expect_Robot_Speed.Y = 0;
	Expect_Robot_Speed.Z = 0;
	//pub_Float_Vector3(Shaking_Hands, 1000, 100, 100);
}

Q_SLOT void boostSeriport::onSpeed()
{
	Expect_Robot_Speed.X += 0.2;
	Expect_Robot_Speed.Y += 0.2;
	Expect_Robot_Speed.Z += 0.2;
	//pub_Float_Vector3(Shaking_Hands, 1000, 100, 100);
}

Q_SLOT void boostSeriport::onSub()
{
	Expect_Robot_Speed.X -= 0.2;
	Expect_Robot_Speed.Y -= 0.2;
	Expect_Robot_Speed.Z -= 0.2;
}

Q_SLOT void boostSeriport::onCtrl()
{
	QString strObj = sender()->objectName();
	if (strObj == "d_btn") {
		keyboardLoop(KEY_D);
	}
	if (strObj == "a_btn") {
		keyboardLoop(KEY_A);
	}
	if (strObj == "w_btn") {
		keyboardLoop(KEY_W);
	}
	if (strObj == "s_btn") {
		keyboardLoop(KEY_S);
	}
}

void boostSeriport::startWheel()
{
	pub_Float_Vector3(Set_Robot_Speed, 0, 0, 0);
	pub_Command(Read_Global_Coordinate);
}

void boostSeriport::keyboardLoop(KEBOARDCMD keyval)
{

	double max_tv = walk_val_;
	double max_rv = yaw_rate_;
	bool dirty = false;
	int speed = 0;
	int turn = 0;

	switch (keyval)
	{
	case KEY_W:/*'W':*///前进
		max_tv = walk_val_;
		speed = 1;
		turn = 0;
		dirty = true;
		break;
	case KEY_S/*'S'*/://后退
		max_tv = walk_val_;
		speed = -1;
		turn = 0;
		dirty = true;
		break;
	case KEY_A/*'A'*/://左转
		max_rv = yaw_rate_;
		speed = 0;
		turn = 1;
		dirty = true;
		break;
	case KEY_D/*'D'*/://右转
		max_rv = yaw_rate_;
		speed = 0;
		turn = -1;
		dirty = true;
		break;
	default:
		max_tv = walk_val_;
		max_rv = yaw_rate_;
		speed = 0;
		turn = 0;
		dirty = false;
	}
	Expect_Robot_Speed.X = speed * max_tv;
	Expect_Robot_Speed.Y = speed * max_tv;
	Expect_Robot_Speed.Z = turn * max_rv;
	pub_Float_Vector3(Set_Robot_Speed, Expect_Robot_Speed.X, Expect_Robot_Speed.Y, Expect_Robot_Speed.Z);
}

void boostSeriport::pub_Command(COMMAND command)
{
	DataGramPtr pDatagram = boost::make_shared<DataGram>();
	pDatagram->sender = MY_ID;
	pDatagram->receiver = ROBOT1_ID;
	pDatagram->data.resize(1, 0);
	uint8_t *pData = pDatagram->data.data();
	pData[0] = (uint8_t)command;
	DataGram_Callback(pDatagram);
}

void boostSeriport::pub_Float_Vector3(COMMAND command, float x1, float x2, float x3)
{
	DataGramPtr pDatagram = boost::make_shared<DataGram>();
	pDatagram->sender = MY_ID;
	pDatagram->receiver = ROBOT1_ID;
	pDatagram->data.resize(13, 0);
	uint8_t *pData = pDatagram->data.data();
	pData[0] = (uint8_t)command;
	*(float*)(pData + 1) = x1;
	*(float*)(pData + 5) = x2;
	*(float*)(pData + 9) = x3;
	DataGram_Callback(pDatagram);
}
