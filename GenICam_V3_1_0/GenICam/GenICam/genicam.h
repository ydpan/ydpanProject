#ifndef GENICAM_H
#define GENICAM_H

#include <QtWidgets/QMainWindow>
#include "ui_genicam.h"

class GenICam : public QMainWindow
{
	Q_OBJECT

public:
	GenICam(QWidget *parent = 0);
	~GenICam();

private:
	Ui::GenICamClass ui;
};

#endif // GENICAM_H
