#include "stytlegraphview.h"
#include "QGraphicsScene.h"

StytleGraphView::StytleGraphView(QWidget *parent /*= 0*/) :QGraphicsView(parent)
, mpImgItem(NULL)
, mpInfoLabel(NULL)
, mpImg(NULL)
, mImgScaleFunc(NULL)
, m_scale(1.0)
, mIsFixedSizeScale(false)
, mFixedSizeScale(1.0)
, mIsAutoResetTransForm(true)
{
	QGraphicsScene *pScene = new QGraphicsScene;
	setScene(pScene);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

StytleGraphView::StytleGraphView(QGraphicsScene *scene, QWidget *parent /*= 0*/) :QGraphicsView(scene,parent)
, mpImgItem(NULL)
, mpInfoLabel(NULL)
, mpImg(NULL)
, mImgScaleFunc(NULL)
, m_scale(1.0)
, mIsFixedSizeScale(false)
, mFixedSizeScale(0.5)
, mIsAutoResetTransForm(true)
{
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

StytleGraphView::~StytleGraphView()
{
	if (mpImg)
	{
		delete mpImg;
		mpImg = NULL;
	}
}

void StytleGraphView::mouseMoveEvent(QMouseEvent *event)
{
	QGraphicsView::mouseMoveEvent(event);
	if (mpImgItem){
		QPoint curpos = (mpImgItem->mapFromScene(this->mapToScene(event->pos()))).toPoint();// +
			//QPointF(mImgOrSize.width() / 2.0, mImgOrSize.height() / 2.0)).toPoint();// +
			//QPointF(-0.5, -0.5)).toPoint();
		if (mpInfoLabel)
			selectPixel(curpos);
		else
			emit pixelSelected(curpos);
	}
	if (mpImgItem &&mpImgItem->isSelected())
	{
		QPointF pos = mpImgItem->scenePos();
		emit imgMoved(pos);
	}
}

void StytleGraphView::mouseReleaseEvent(QMouseEvent *event)
{	
	
	if (mpImgItem)
		mpImgItem->setSelected(false);
	QGraphicsView::mouseReleaseEvent(event);
}

void StytleGraphView::wheelEvent(QWheelEvent *event)
{
	Qt::FocusPolicy policy = this->focusPolicy();
	if (mpImgItem)
	{
		qreal numDegrees = event->angleDelta().y() / 120;
		if (!isFixedSizeScale())
		{
			qreal newScale = transform().m11();
			if (numDegrees > 0)
				newScale *= std::pow(1.1, numDegrees);
			else if (numDegrees < 0)
				newScale *= std::pow(0.9, -numDegrees);
			if (newScale < 0.05)
				newScale = 0.05;
			mpImgItem->setTransformOriginPoint(0, 0);
			mpImgItem->setTransformationMode(Qt::SmoothTransformation);
			scalImg2(newScale);
		}
		else
		{
			qreal newScale = fixedSizeScale();
			if (numDegrees>0)
				newScale *= std::pow(1.1, numDegrees);
			else if (numDegrees < 0)
				numDegrees *= std::pow(0.9, -numDegrees);
			if (newScale>1)
				newScale = 1;
			setFixedSizeScale(newScale);
			scaleImageWithFixedViewSize();
		}
	}
}

void StytleGraphView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
}

void StytleGraphView::closeEvent(QCloseEvent *event)
{
	deleteLater();
}

void StytleGraphView::clear()
{
	m_scale = 1.0f;
	m_channel = "";
	scene()->clear();
	mpImgItem = NULL;
	if (mpImg)
	{
		delete mpImg;
		mpImg = NULL;
	}
}

void StytleGraphView::scalImg2(qreal scale)
{
	m_scale = scale;
	QTransform t;
	t.scale(scale, scale);
	this->setTransform(t);
	int nw = width();
	int nh = height();
	QPointF center = this->mapToScene(width() / 2, height() / 2);
	emit imgScaled(this->transform(), center);
}

Q_SLOT void StytleGraphView::scaleImg(QTransform tf, QPoint center)
{
	this->resetTransform();
	this->setTransform(tf);
	this->centerOn(center);
}

Q_SLOT void StytleGraphView::moveImg(QPoint pos)
{
	if (mpImgItem)
		mpImgItem->setPos(pos);
}

Q_SLOT void StytleGraphView::selectPixel(QPoint pos)
{
	if (pos.x() < 0 || pos.y() < 0 || pos.x() > mImgOrSize.width() || pos.y() > mImgOrSize.height())
	{
		mpInfoLabel->setText(QString("out of image..."));
		return;
	}
	QRgb pixel = mpImg->pixel(pos);
	QColor rgb(pixel);
	QColor hsv = rgb.toHsv();
	//QString infoStr("Position:[%1,%2] Color(RGB):[%3,%4,%5] (hsv):[%6,%7,%8]");
	//infoStr = infoStr.arg(pos.x()).arg(pos.y()).arg(rgb.red()).arg(rgb.green()).arg(rgb.blue())
	//	.arg(hsv.hue() / 2).arg(hsv.saturation()).arg(hsv.value());
	QString infoStr("Position:[%1,%2]");
	infoStr = infoStr.arg(pos.x()).arg(pos.y());
	mpInfoLabel->setText(infoStr);
}

Q_SLOT void StytleGraphView::scaleImageWithFixedViewSize()
{
	if (!mpImg)
	{
		return;
	}
	QImage scaledImage;
	if (!imgScaleFunc())
	{
		scaledImage = mpImg->scaled(mpImg->width()*mFixedSizeScale, mpImg->height()*mFixedSizeScale,
			Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		mpImgItem->setPixmap(QPixmap::fromImage(scaledImage));
	}
	else
	{
		scaledImage = mImgScaleFunc(*mpImg, mFixedSizeScale, Qt::SmoothTransformation);
		mpImgItem->setPixmap(QPixmap::fromImage(mImgScaleFunc(scaledImage,
			1.0 / mFixedSizeScale, Qt::FastTransformation)));
	}
	emit imgScaledWidthFixedSize(fixedSizeScale());
}

Q_SLOT void StytleGraphView::enlargeImg()
{
	if (mpImgItem)
	{
		qreal numDegrees = 2;
		if (!isFixedSizeScale())
		{
			qreal newScale = transform().m11();
			if (numDegrees > 0)
				newScale *= std::pow(1.1, numDegrees);
			else if (numDegrees < 0)
				newScale *= std::pow(0.9, -numDegrees);
			mpImgItem->setTransformOriginPoint(0, 0);
			scalImg2(newScale);
		}
		else
		{
			qreal newScale = fixedSizeScale();
			if (numDegrees > 0)
				newScale *= std::pow(1.1, numDegrees);
			else if (numDegrees < 0)
				newScale *= std::pow(0.9, -numDegrees);

			if (newScale > 1.0) newScale = 1.0;

			setFixedSizeScale(newScale);

			scaleImageWithFixedViewSize();
		}
	}
}

Q_SLOT void StytleGraphView::zoomOutImg()
{
	if (mpImgItem)
	{
		qreal numDegrees = -2;
		if (!isFixedSizeScale())
		{
			qreal newScale = transform().m11();
			if (numDegrees > 0)
				numDegrees *= std::pow(1.1, numDegrees);
			else if (numDegrees < 0)
				numDegrees *= std::pow(0.9, -numDegrees);
			mpImgItem->setTransformOriginPoint(0, 0);
			scalImg2(newScale);
		}
		else
		{
			qreal newScale = fixedSizeScale();
			if (numDegrees>0)
				newScale *= std::pow(1.1, numDegrees);
			else if (numDegrees < 0)
				newScale *= std::pow(0.9, -numDegrees);

			if (newScale>1.0)
				newScale = 1.0;
			setFixedSizeScale(newScale);
			scaleImageWithFixedViewSize();
		}
	}
}

QGraphicsPixmapItem * StytleGraphView::setImg(QImage& img)
{
	if (mpImg)
		delete mpImg;
	mpImg = new QImage(img);

	if (!mpImgItem)
	{
		mpImgItem = scene()->addPixmap(QPixmap::fromImage(img.scaled(img.width()*mFixedSizeScale, img.height()*mFixedSizeScale,
			Qt::KeepAspectRatio, Qt::SmoothTransformation)));
		QPointF position = mpImgItem->pos();
	}
	else
	{
		QImage scaledImage = img.scaled(img.width()*mFixedSizeScale, img.height()*mFixedSizeScale,
			Qt::KeepAspectRatio, Qt::SmoothTransformation);
		
		mpImgItem->setPixmap(QPixmap::fromImage(scaledImage));
	}

	mImgOrSize = img.size();
	int width = this->width();
	int Imgwidth = mImgOrSize.width();
	int hight = this->height();
	int Imghight = mImgOrSize.height();
	qreal widscal = width*1.0 /Imgwidth;
	qreal higscal = hight*1.0 / Imghight;
	qreal finalScal = 1.0f;
	if (widscal < higscal)
		finalScal = widscal;
	else
		finalScal = higscal;
	if (isAutoResetTransform())
	{
		if (finalScal > 1.0f)
			finalScal = 1.0f;
		qreal scale = finalScal;// 1.0f;
		scalImg2(scale);
		this->centerOn(0, 0);
	}
	mpImgItem->setFlag(QGraphicsItem::ItemIsMovable);
	return mpImgItem;
}

 QGraphicsPixmapItem * StytleGraphView::setImg(QPixmap img)
 {
	 QImage pImg = img.toImage();
	 return setImg(pImg);
 }

