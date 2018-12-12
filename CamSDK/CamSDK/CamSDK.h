#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CamSDK.h"

class CamSDK : public QMainWindow
{
	Q_OBJECT

public:
	CamSDK(QWidget *parent = Q_NULLPTR);

private:
	Ui::CamSDKClass ui;
};
