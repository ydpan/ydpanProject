#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mudoDP.h"

class mudoDP : public QMainWindow
{
	Q_OBJECT

public:
	mudoDP(QWidget *parent = Q_NULLPTR);

private:
	Ui::mudoDPClass ui;
};
