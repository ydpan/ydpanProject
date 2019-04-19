#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SDKTest.h"

class SDKTest : public QMainWindow
{
	Q_OBJECT

public:
	SDKTest(QWidget *parent = Q_NULLPTR);

private:
	Ui::SDKTestClass ui;
};
