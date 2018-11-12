/******************************************************************************
Copyright(C):2017 hzleaper
FileName:qpixmapBar.h  qpixmapBar.cpp
Author:bob.pan
Email:bob.pan@hzleaper.com
Tools:vs2013 pc on company
Created:2017/09/30
History:2017/09/30
*******************************************************************************/
#ifndef QPIXMAPBAR_H
#define QPIXMAPBAR_H

#include <QFrame>
#include <QPainter>
#include <QPaintEvent>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollBar>
#include <QTimer>
#include "qboxlayout.h"

#define ITEM_START_ANGLE 270
#define ITEM_D_ANGLE 2
#define ITEM_D_ZOOMING_FACTOR 0.1
#define UPDATE_TIMER_INTERVAL 10 // ms
#define ITEM_COUNTER_TIMER_INTERVAL 200
#define SIZE_HINT QSize(300,300)

#define ITEM_WIDTH  120
#define ITEM_HEIGHT 90
#define BG_START_COLOR QColor(89,89,89)
#define BG_END_COLOR QColor(90,90,90)

#define ITEM_START_COLOR ITEM_END_COLOR.lighter()
#define ITEM_END_COLOR QColor(62,135,166)

#define ITEM_START_COLOR_A ITEM_END_COLOR_A.lighter()
#define ITEM_END_COLOR_A QColor(221,221,221)

#define HOVER_OPACITY 0.2
#define HOVER_START_COLOR HOVER_END_COLOR.lighter()
#define HOVER_END_COLOR QColor(98,139,21)

#define HOVER_TEXT_COLOR Qt::white
#define SELECT_TEXT_COLOR Qt::white
#define NORMAL_TEXT_COLOR Qt::black

#define RECT_RADIUS 4

class QPixmapBarItem
{
public:
	QPixmapBarItem(const QString& strItemText = QString()) : m_strData(strItemText), m_ZoomingFactor(0)
	{ 
		pPixmap = new QPixmap; 
	};
	QPixmapBarItem(const QString& strItemText,const QString &filepath = QString()) : m_strData(strItemText), m_ZoomingFactor(0)
	{ 
		pPixmap = new QPixmap; 
		pPixmap->load(filepath);
	};
	QPixmapBarItem(const QPixmap &pPix,const QString& strItemText = QString()): m_strData(strItemText), m_ZoomingFactor(0)
	{
		pPixmap = new QPixmap;
		*pPixmap = pPix;
	};
	QPixmapBarItem(const QImage &nImg,const QString& strItemText = QString()) :m_strData(strItemText), m_ZoomingFactor(0)
	{
		pPixmap = new QPixmap;
		*pPixmap = QPixmap::fromImage(nImg);
	};
	~QPixmapBarItem(){
		if (pPixmap){ 
			delete pPixmap; 
			pPixmap = NULL; 
		}
	}

public:// public interfaces
	void setPic(QPixmap &pix){ *pPixmap = pix; };
	void setPic(QString filepath){
		if (pPixmap){
			pPixmap->load(filepath);
		}
	};
	QPixmap *getPix() const{ return pPixmap; };
	void setText(const QString& str){ m_strData = str; };
	QString getText() const{ return m_strData; };
	void updateZoomingFactor(){
		m_ZoomingFactor += ITEM_D_ZOOMING_FACTOR;
		if (m_ZoomingFactor > 1.0){
			m_ZoomingFactor = 1.0;
		}
	};
	bool finishZoomingFactor() const{ return m_ZoomingFactor == 1.0; };
	void resetZoomingFactor(){ m_ZoomingFactor = 0.0; };
	qreal getZoomingFactor() const{ return m_ZoomingFactor; };
	bool inWrongZoomingPosition(){ return m_ZoomingFactor < 1.0; };
private:
	QString m_strData;
	qreal m_ZoomingFactor;
	QPixmap *pPixmap;
};

class QPixmapView : public QWidget
{
	Q_OBJECT
public:
	explicit QPixmapView(QWidget *parent = 0);

public:	/// public interfaces
	void addItem(const QString &strItemName, const QString &filePath = QString());
	void addItem(const QPixmap &pPixmap, const QString &strItemName = QString());
	void addItem(const QImage &pImg, const QString &strItemName = QString());
	void addItemByLimit(const QString &strItemName, const QString &filePath = QString());
	void addItemByLimit(const QPixmap &pPixmap, const QString &strItemName = QString());
	void addItemByLimit(const QImage &pImg, const QString &strItemName = QString());

	void clear();
	void render();
	void removeFirst();

protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void leaveEvent(QEvent *event);
	void showEvent(QShowEvent *event);
	void wheelEvent(QWheelEvent *event);
private:// painting functions
	void drawBackground(QPainter* painter);
	void drawItems(QPainter* painter);
	void drawHoverRect(QPainter* painter);
private:// private utility functoins
	void initVars();
	void initSettings();
	void ResetItemCount();
	void updateTimer();
	void wheelUp();
	void wheelDown();
private:
	QVector<QPixmapBarItem*> m_IIVec;
	int m_currIndex;
	int m_selectedIndex;
	int m_VisibleItemCnt;
	int m_ItemCounter;
	bool m_bAllTimersDone;
	QRectF m_HoverRect;
	QString m_strHoverText;
	QTimer* m_RotateTimer;
	QTimer* m_ItemCountTimer;
	int m_VectorSize;
signals:
	void sgSetMaximum(int max);
	void sgSetCurrentIndex(int currIndex);
	void sgItemClicked(const QString& str);
public:
	Q_SLOT void setCurrentIndex(int curIndex);
	Q_SLOT void DoRotation();
	Q_SLOT void UpdateItemCount();
	void setIndex(int nIndex);
};

class QPixmapListBar : public QFrame
{
	Q_OBJECT
public:
	explicit QPixmapListBar(QWidget *parent = 0);
	~QPixmapListBar();
public:
	/// proxy public interfaces for PYFlashListPrivate
	void addItem(const QString& str);
	void addItem(const QString &str, const QString &filename);
	void addItemByLimit(const QString &str, const QString &filename);
	void clear();
	void render();
	void removeFirst();
protected:
	QSize sizeHint() const
	{
		return QSize(SIZE_HINT);
	}
	void resizeEvent(QResizeEvent *e);
private:// private utility functoins
	void initWgts();
	void initStgs();
	void initConns();
	void calcGeo();
private Q_SLOTS:
	void setMaximum(int max);
private:
	QVBoxLayout* mainLayout;
	QScrollBar* pScrollbar;
	QPixmapView* pPixmapView;
signals:
	void sgSetCurrentIndex(int currIndex);
	void sgItemClicked(const QString& str);
public slots:

};

#endif // QPIXMAPBAR_H
