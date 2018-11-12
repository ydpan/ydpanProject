#ifndef STYTLEGRAPHVIEW_H
#define STYTLEGRAPHVIEW_H

#include <QGraphicsView>
#include "QGraphicsScene"
#include "qevent.h"
#include "QGraphicsPixmapItem"
#include "qlabel.h"
#include "qimage.h"
#include "qtransform.h"

typedef QImage(*ImageScaleFun)(QImage, double, int);
class StytleGraphView : public QGraphicsView
{
	Q_OBJECT

public:
	StytleGraphView(QWidget *parent = 0);
	StytleGraphView(QGraphicsScene *scene, QWidget *parent = 0);
	~StytleGraphView();
public:
	QGraphicsPixmapItem *setImg(QImage& img);
	QGraphicsPixmapItem *setImg(QPixmap img);
	void setInfoLabel(QLabel *pLabel){ mpInfoLabel = pLabel; };
	const QImage* img(){ return mpImg; };
	QGraphicsPixmapItem* imgItem(){ return mpImgItem; };
	
	const qreal getScale(void) const{ return m_scale; };
	void setChannel(const QString& channel){ m_channel = channel; };
	QString getChannel() const{ return m_channel; };
	bool isFixedSizeScale() const{ return mIsFixedSizeScale; };
	void setIsFixedSizeScale(bool val){ mIsFixedSizeScale = val; };
	qreal fixedSizeScale() const { return mFixedSizeScale; };
	void setFixedSizeScale(qreal val){ mFixedSizeScale = val; };

	ImageScaleFun imgScaleFunc() const{ return mImgScaleFunc; };
	void setImgScaleFunc(ImageScaleFun val){ mImgScaleFunc = val; };

	bool isAutoResetTransform() const { return mIsAutoResetTransForm; };
	void setIsAutoResetTransform(bool val){ mIsAutoResetTransForm = val; };
protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void clear();
signals:
	void imgScaled(QTransform tf, QPointF center);
	void imgMoved(QPointF pos);
	void pixelSelected(QPoint pos);
	void imgScaledWidthFixedSize(qreal scale);
public:
	Q_SLOT void scaleImg(QTransform tf, QPoint center);
	Q_SLOT void moveImg(QPoint pos);
	Q_SLOT void selectPixel(QPoint pos);
	Q_SLOT void scaleImageWithFixedViewSize();
	
	Q_SLOT void enlargeImg();
	Q_SLOT void zoomOutImg();
protected:
	QGraphicsPixmapItem *mpImgItem;
	QSize mImgOrSize;
	QLabel *mpInfoLabel;
	QImage *mpImg;
	void scalImg2(qreal scale);
	qreal m_scale;
	QString m_channel;
	
	bool mIsFixedSizeScale;
	qreal mFixedSizeScale;
	bool mIsAutoResetTransForm;

	ImageScaleFun mImgScaleFunc;
};

#endif // STYTLEGRAPHVIEW_H
