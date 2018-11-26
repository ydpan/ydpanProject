#ifndef _BOOSTSERIALPORT_H_
#define _BOOSTSERIALPORT_H_

#include <QtWidgets/QMainWindow>
#include "ui_boostSeriport.h"
#include "SerialPortDevice.h"
#include <QTimer>
#include <QMap>
#include <QVariant>
enum KEBOARDCMD {
	KEY_W, KEY_S, KEY_A, KEY_D
};
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
	Q_SLOT void onStart();
	Q_SLOT void onSpeed();
	Q_SLOT void onSub();
	Q_SLOT void onCtrl();
	Q_SLOT void onT();
	void startWheel();
	void keyboardLoop(KEBOARDCMD keyval);
private:
	double walk_val_{ 0.15 };
	double run_val_{ 1.0 };
	double yaw_rate_{ 1.0 };
	double yaw_rate_run_{1.5};

	typedef struct ROBOT_FLOAT_XYZ {
		float  X;
		float  Y;
		float  Z;
	} ROBOT_FLOAT_XYZ;

	typedef struct ROBOT_MOTOR {
		float  M1;
		float  M2;
		float  M3;
	} ROBOT_MOTOR;

	ROBOT_FLOAT_XYZ  Expect_Global_Speed, Expect_Robot_Speed;
	ROBOT_MOTOR      Expect_Motor_Speed;
	ROBOT_FLOAT_XYZ  Mesure_Global_Coordinate, Mesure_Robot_Coordinate;  //测量的坐标 X Y单位为M  Z的单位为弧度
	ROBOT_FLOAT_XYZ  Mesure_Global_Speed, Mesure_Robot_Speed;
	ROBOT_MOTOR Mesure_Motor_Speed, Mesure_Motor_Distance;                 //电机速度 单位是角度/S  测量的里程值 单位是M
	ROBOT_FLOAT_XYZ  Mesure_odom_filter;

	enum COMMAND {
		Shaking_Hands, Read_BSP_Info
		, Set_Global_Speed, Read_Global_Speed, Set_Robot_Speed, Read_Robot_Speed
		, Set_Motor_Speed, Read_Motor_Speed, Read_Motor_Mileage
		, Read_Global_Coordinate, Clear_Coordinate_Data
		, Set_Arm_1, Read_Arm_1, Set_Arm_2, Read_Arm_2
		, Set_Head_1, Read_Head_1, Set_Head_2, Read_Head_2
	}HF_Command_State;

	uint8_t MY_ID{ 0x01 };
	uint8_t ROBOT1_ID{ 0x11 };


	void pub_Command(COMMAND command);
	void pub_Float_Vector3(COMMAND command, float x1, float x2, float x3);
signals:
	void sgRecvData(QVariantMap recvData);
private:
	Ui::boostSeriportClass ui;

	SerialPortParams m_serialParams;
	int m_serialPortTimeOut{100};
	boost::shared_ptr<SerialPortDevice> m_pSerialDevice;

	QTimer m_timer;
	QTimer m_timer2;
	COMMAND _command_state{ Shaking_Hands };
};

#endif
