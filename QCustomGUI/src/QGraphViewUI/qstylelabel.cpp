#include "qstylelabel.h"
#include "QAbstractItemDelegate"
#include "QColor"
#include "QPainterPath"
#include "QPen.h"
#include "QPainter.h"
QStyleLabel::QStyleLabel(QWidget *parent)
	: QLabel("",parent)
{

}

QStyleLabel::~QStyleLabel()
{

}

void QStyleLabel::paintEvent(QPaintEvent *event)
{
	QRect rc = event->rect();
	QPainter painter(this);
	painter.setBrush(QColor(100, 0, 42,50));
	painter.save();

	QPen pen(Qt::black);
	painter.setPen(pen);
	painter.drawRect(rc);
	QFontMetrics fntMetrics(painter.font());
	painter.drawText(rc.left() + 10, rc.top() + 32, "testpainter");
	painter.drawText(rc.left() + 10, rc.top() + 64, "testpainter2");
	painter.restore();
}
