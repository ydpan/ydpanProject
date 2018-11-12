#ifndef QSTYLELABEL_H
#define QSTYLELABEL_H

#include <QLabel>
#include "qevent.h"

class QStyleLabel : public QLabel
{
	Q_OBJECT

public:
	QStyleLabel(QWidget *parent = 0);
	~QStyleLabel();

private:
protected:
	virtual void paintEvent(QPaintEvent *event);
};

#endif // QSTYLELABEL_H
