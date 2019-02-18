#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CamSDK.h"
#include <boost/asio.hpp>
using namespace  boost;

class CamSDK : public QMainWindow
{
	Q_OBJECT

public:
	CamSDK(QWidget *parent = Q_NULLPTR);
	Q_SLOT void onFindDevice();
private:
	Ui::CamSDKClass ui;
	int m_nMsgNr{ 0 };
};
