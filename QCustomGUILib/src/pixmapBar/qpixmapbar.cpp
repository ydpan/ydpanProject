#include "qpixmapbar.h"

QPixmapListBar::QPixmapListBar(QWidget *parent) 
	:QFrame(parent)
{
	initWgts();
	initStgs();
	initConns();
}

QPixmapListBar::~QPixmapListBar()
{
	if (pPixmapView){
		pPixmapView->clear();
		delete pPixmapView;
		pPixmapView = NULL;
	}
}

void QPixmapListBar::initWgts()
{
	setMaximumHeight(ITEM_HEIGHT + 30);
	mainLayout = new QVBoxLayout(this);
	pScrollbar = new QScrollBar(Qt::Horizontal, this);
	pPixmapView = new QPixmapView(this);
	pPixmapView->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(pPixmapView);
	mainLayout->addWidget(pScrollbar);
	//mainLayout->addStretch();
	setLayout(mainLayout);
}

void QPixmapListBar::initStgs()
{
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	pScrollbar->setRange(0, 0);
	pScrollbar->hide();
}

void QPixmapListBar::initConns()
{
	connect(pScrollbar, SIGNAL(valueChanged(int)), pPixmapView, SLOT(setCurrentIndex(int)));
	connect(pPixmapView, SIGNAL(sgSetMaximum(int)), this, SLOT(setMaximum(int)));
	connect(pPixmapView, SIGNAL(sgSetCurrentIndex(int)), pScrollbar, SLOT(setValue(int)));
	connect(pPixmapView, SIGNAL(sgItemClicked(QString)), this, SIGNAL(sgItemClicked(QString)));
}

void QPixmapListBar::calcGeo()
{

}

void QPixmapListBar::setMaximum(int max){
	if (max == 0)
	{
		pScrollbar->hide();
	}
	else{
		pScrollbar->show();
	}
	pScrollbar->setRange(0, max);
}

void QPixmapListBar::resizeEvent(QResizeEvent *e)
{
	
}

void QPixmapListBar::addItem(const QString &str,const QString &filename)
{
	pPixmapView->addItem(str, filename);
}

void QPixmapListBar::addItem(const QString &str)
{
	pPixmapView->addItem(str);
}

void QPixmapListBar::addItemByLimit(const QString &str, const QString &filename)
{
	pPixmapView->addItemByLimit(str, filename);
}

void QPixmapListBar::clear()
{
	pPixmapView->clear();
}

void QPixmapListBar::render()
{
	pPixmapView->render();
}

void QPixmapListBar::removeFirst()
{
	pPixmapView->removeFirst();
}

QPixmapView::QPixmapView(QWidget* parent) 
	:QWidget(parent), m_VectorSize(4)
{
	setMinimumHeight(ITEM_HEIGHT + 10);
	setMaximumHeight(ITEM_HEIGHT + 50);
	initVars();
	initSettings();
}

void QPixmapView::initVars()
{
	m_currIndex = 0;
	m_VisibleItemCnt = 0;
	m_ItemCounter = 0;

	m_bAllTimersDone = false;

	m_RotateTimer = new QTimer(this);
	m_RotateTimer->setInterval(UPDATE_TIMER_INTERVAL);
	connect(m_RotateTimer, SIGNAL(timeout()), this, SLOT(DoRotation()));

	m_ItemCountTimer = new QTimer(this);
	m_ItemCountTimer->setInterval(ITEM_COUNTER_TIMER_INTERVAL);
	connect(m_ItemCountTimer, SIGNAL(timeout()), this, SLOT(UpdateItemCount()));
}

void QPixmapView::initSettings()
{
	setMouseTracking(this);
}

void QPixmapView::ResetItemCount()
{
	// m_VisibleItemCnt = height()/ ITEM_HEIGHT + 1;
	m_VisibleItemCnt = width() / ITEM_WIDTH;
	int InvisibleItemCnt = m_IIVec.count() - m_VisibleItemCnt;
	if (InvisibleItemCnt < 0)
		InvisibleItemCnt = 0;
	emit (sgSetMaximum(InvisibleItemCnt));
}

void QPixmapView::updateTimer()
{
	int Guard = 0;
	for (int index = m_currIndex; index < m_IIVec.count(); index++){
		if (Guard++ > m_VisibleItemCnt){
			break;
		}

		if (!m_IIVec.at(index)->finishZoomingFactor()){
			if (!m_RotateTimer->isActive()){
				m_RotateTimer->start();
				m_ItemCountTimer->start();
				break;
			}
		}
	}
}

void QPixmapView::wheelUp()
{
	if (--m_currIndex < 0){
		m_currIndex = 0;
	}
	update();
	emit (sgSetCurrentIndex(m_currIndex));
	updateTimer();
}

void QPixmapView::wheelDown()
{
	if (++m_currIndex + m_VisibleItemCnt > m_IIVec.count())
	{
		m_currIndex = m_IIVec.count() - m_VisibleItemCnt;
	}
	if (m_currIndex <= 0)
		m_currIndex = 0;
	update();
	emit (sgSetCurrentIndex(m_currIndex));
	updateTimer();
}

void QPixmapView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing);
	drawBackground(&painter);
	drawItems(&painter);
	drawHoverRect(&painter);
}

void QPixmapView::mouseMoveEvent(QMouseEvent *e)
{
	if (!m_bAllTimersDone){
		return;
	}
	int ItemCnt = e->pos().x() / ITEM_WIDTH;
	if (m_currIndex + ItemCnt < m_IIVec.count()){
		QPointF topLeft(ItemCnt*ITEM_WIDTH, (height()-ITEM_HEIGHT)/2);
		QPointF bottomRight((ItemCnt + 1)*ITEM_WIDTH, (height() - ITEM_HEIGHT) / 2 + ITEM_HEIGHT);
		m_HoverRect = QRectF(topLeft, bottomRight);
		m_strHoverText = m_IIVec.at(m_currIndex + ItemCnt)->getText();
		update();
	}
}

void QPixmapView::mousePressEvent(QMouseEvent *e)
{
	if (!m_bAllTimersDone)
	{
		return;
	}
	//int ItemCnt = e->pos().y()/ITEM_HEIGHT;
	int ItemCnt = e->pos().x() / ITEM_WIDTH;
	if (m_currIndex + ItemCnt < m_IIVec.count())
	{
		m_selectedIndex = m_currIndex + ItemCnt;
		QString strText = m_IIVec.at(m_selectedIndex)->getText();
		emit(sgItemClicked(strText));
	}
	update();
}

void QPixmapView::resizeEvent(QResizeEvent *event)
{
	ResetItemCount();
	updateTimer();
	m_VectorSize = m_VisibleItemCnt;
}

void QPixmapView::leaveEvent(QEvent *event)
{
	m_HoverRect = QRectF();
	update();
}
void QPixmapView::showEvent(QShowEvent *event)
{
	ResetItemCount();
}

void QPixmapView::wheelEvent(QWheelEvent *e)
{
	if (e->delta() > 0){
		wheelUp();
	}
	else{
		wheelDown();
	}
}

void QPixmapView::drawBackground(QPainter *painter)
{
	painter->save();
	QLinearGradient BgGradient(QPoint(0, 0), QPoint(0, height()));
	BgGradient.setColorAt(0.0, BG_START_COLOR);
	BgGradient.setColorAt(1.0, BG_END_COLOR);
	painter->setBrush(BgGradient);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->drawRect(rect());
	painter->restore();
}

void QPixmapView::drawItems(QPainter *painter)
{
	if (m_IIVec.count() == 0)
	{
		return;
	}
	painter->save();
	int Guard = 0;
	qreal initY = 0;
	qreal initX = 0;
	for (int index = m_currIndex; index < m_IIVec.count(); index++)
	{
		if (Guard++ > m_VisibleItemCnt)
		{
			break;
		}

		QPointF topLeft(initX, initY);
		QPointF bottomRight(initX + ITEM_WIDTH, height());//ITEM_HEIGHT
		QRectF ItemRect(topLeft, bottomRight);
		painter->save();

		QTransform t;
		t.translate(ItemRect.center().x(), ItemRect.center().y());
		t.scale(m_IIVec.at(index)->getZoomingFactor(), m_IIVec.at(index)->getZoomingFactor());
		painter->setTransform(t);

		QPointF TedTopLeft(-ITEM_WIDTH / 2, -ITEM_HEIGHT / 2);
		QPointF TedBottomRight(ITEM_WIDTH / 2, ITEM_HEIGHT / 2);
		QRectF TedRect(TedTopLeft, TedBottomRight);

		QPixmap *mPix = m_IIVec.at(index)->getPix();//获取需要显示的图像
		/*以下是为了居中显示图像数据，可根据不同的图像尺寸进行缩放*/
		int dhight = mPix->height();
		int dwidth = mPix->width();
		double nRate = mPix->width()*1.0 / mPix->height();
		int scarew = ITEM_WIDTH;
		int scareh = ITEM_WIDTH*1.0/nRate;
		QRect qrect = TedRect.toRect();
		if (scareh > ITEM_HEIGHT)
		{
			scarew = ITEM_HEIGHT;
			scareh = ITEM_HEIGHT*1.0 / nRate;
			
			int x1 = (ITEM_WIDTH - scarew)/2;
			qrect.setX(qrect.x() + x1);
			qrect.setWidth(scareh);
		}
		else
		{
			int y1 = (ITEM_HEIGHT - scareh) / 2;
			qrect.setY(qrect.y() + y1);
			qrect.setHeight(scareh);
		}
		/*图像绘制图像坐标获取完成*/
		/*此处绘制边框和背景*/
		painter->setPen(QPen(QColor(139, 139, 139), 1));
		painter->setBrush(QColor(177, 177, 177));
		painter->drawRect(TedRect);
		painter->setRenderHint(QPainter::Antialiasing, true);
		
		/*此处绘制图片*/
		painter->drawPixmap(qrect, mPix->scaled(scarew, scareh));
		//painter->setPen(NORMAL_TEXT_COLOR);
		painter->setPen(Qt::yellow);
		/*绘制文字*/
		QFont BoldFont;
		BoldFont.setBold(true);
		painter->setFont(BoldFont);

		//painter->drawText(TedRect, m_IIVec.at(index).getText(), Qt::AlignVCenter | Qt::AlignHCenter);
		painter->drawText(TedRect, m_IIVec.at(index)->getText(), Qt::AlignLeft | Qt::AlignBottom);
		painter->restore();

		initX += ITEM_WIDTH;
	}
	painter->restore();
}

void QPixmapView::drawHoverRect(QPainter *painter)
{//绘制焦点时的状态
	if (m_HoverRect.isEmpty() && !m_bAllTimersDone)
	{
		return;
	}

	painter->save();
	//painter->setPen(Qt::NoPen);
	painter->setPen(QPen(QColor(100, 255, 0),2));
	painter->setBrush(QBrush(QColor(177, 177, 177, 88)));
	painter->drawRoundedRect(m_HoverRect, 2, 2);//绘制矩形框 // RECT_RADIUS, RECT_RADIUS);
	
	QFont BoldFont;
	BoldFont.setBold(true);
	painter->setFont(BoldFont);

	painter->setPen(HOVER_TEXT_COLOR);
	//painter->drawText(m_HoverRect, m_strHoverText, Qt::AlignVCenter | Qt::AlignHCenter);
	painter->drawText(m_HoverRect, m_strHoverText, Qt::AlignLeft | Qt::AlignBottom);
	painter->restore();
}

void QPixmapView::DoRotation()
{
	int Guard = 0;
	bool bHasJobUndone = false;
	m_bAllTimersDone = false;
	m_HoverRect = QRectF();

	for (int index = m_currIndex; index < m_IIVec.count()/* && Guard < m_ItemCounter*/; index++)
	{
		if (!m_IIVec.at(index)->finishZoomingFactor())
		{
			bHasJobUndone = true;
			m_IIVec[index]->updateZoomingFactor();
		}

		if (Guard++ > m_VisibleItemCnt)
		{
			break;
		}
	}

	if (!bHasJobUndone)
	{
		m_RotateTimer->stop();
		m_bAllTimersDone = true;
	}
	update();
}

void QPixmapView::UpdateItemCount()
{
	if (++m_ItemCounter > m_VisibleItemCnt)
	{
		if (m_ItemCountTimer->isActive())
		{
			m_ItemCountTimer->stop();
			m_ItemCounter = 0;
		}
	}
}

void QPixmapView::addItem(const QPixmap &pPixmap, const QString &strItemName /*= QString()*/)
{
	QPixmapBarItem *ID = new QPixmapBarItem(pPixmap, strItemName);
	m_IIVec.push_back(ID);

	if (m_RotateTimer->isActive())
	{
		m_RotateTimer->stop();
	}
	m_RotateTimer->start();
}

void QPixmapView::addItem(const QImage &pImg, const QString &strItemName /*= QString()*/)
{
	QPixmapBarItem *ID = new QPixmapBarItem(pImg, strItemName);
	m_IIVec.push_back(ID);

	if (m_RotateTimer->isActive()){
		m_RotateTimer->stop();
	}
	m_RotateTimer->start();
}

void QPixmapView::addItem(const QString &strItemName, const QString &filePath /*= QString()*/)
{
	QPixmapBarItem *ID = new QPixmapBarItem(strItemName, filePath);
	m_IIVec.push_back(ID);

	if (m_RotateTimer->isActive())
	{
		m_RotateTimer->stop();
	}
	m_RotateTimer->start();
}

void QPixmapView::addItemByLimit(const QString &strItemName, const QString &filePath /*= QString()*/)
{
	int nSize = m_IIVec.size();
	while (m_IIVec.size() >= m_VectorSize)//控制显示的数量 删除最先进入vector的items
	{
		removeFirst();
	}
	addItem(strItemName, filePath);
}

void QPixmapView::addItemByLimit(const QPixmap &pPixmap, const QString &strItemName /*= QString()*/)
{
	int nSize = m_IIVec.size();
	while (m_IIVec.size() >= m_VectorSize)//控制显示的数量 删除最先进入vector的items
	{
		removeFirst();
	}
	addItem(pPixmap, strItemName);
}

void QPixmapView::addItemByLimit(const QImage &pImg, const QString &strItemName /*= QString()*/)
{
	int nSize = m_IIVec.size();
	while (m_IIVec.size() >= m_VectorSize)//控制显示的数量 删除最先进入vector的items
	{
		removeFirst();
	}
	addItem(pImg, strItemName);
}

void QPixmapView::clear()
{
	if (m_IIVec.size() > 0){
		while (m_IIVec.size())
		{
			QPixmapBarItem *pIndex = m_IIVec.takeFirst();
			delete pIndex;
			pIndex = NULL;
		}
	}
	ResetItemCount();
	update();
}

void QPixmapView::removeFirst()
{
	if (m_IIVec.size() > 0)
	{
		QPixmapBarItem *pIndex = m_IIVec.takeFirst();
		delete pIndex;
		pIndex = NULL;
	}
	ResetItemCount();
	update();
	updateTimer();
}

void QPixmapView::render()
{
	update();
}

void QPixmapView::setCurrentIndex(int curIndex)
{
	if (curIndex == m_currIndex)
	{
		return;
	}

	m_currIndex = curIndex;

	//m_VisibleItemCnt = height()/ITEM_HEIGHT + 1;
	m_VisibleItemCnt = width() / ITEM_WIDTH + 1;
	int Guard = 0;
	for (int index = m_currIndex; index < m_IIVec.count(); index++)
	{
		m_IIVec[index]->resetZoomingFactor();
		if (Guard++ > m_VisibleItemCnt)
		{
			break;
		}
	}
	m_RotateTimer->start();
	m_ItemCountTimer->start();
}

void QPixmapView::setIndex(int nIndex)
{
	if (!m_bAllTimersDone){
		return;
	}
	int ItemCnt = nIndex;
	if (m_currIndex + ItemCnt < m_IIVec.count()){
		QPointF topLeft(ItemCnt*ITEM_WIDTH, (height() - ITEM_HEIGHT) / 2);
		QPointF bottomRight((ItemCnt + 1)*ITEM_WIDTH, (height() - ITEM_HEIGHT) / 2 + ITEM_HEIGHT);
		m_HoverRect = QRectF(topLeft, bottomRight);
		m_strHoverText = m_IIVec.at(m_currIndex + ItemCnt)->getText();
		update();
	}
}
