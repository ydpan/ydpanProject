#include "cunstomgraphview.h"
#include "QBoxLayout"
#include "stytlegraphview.h"
QTpGraphView::QTpGraphView(QWidget *parent)
	: QFrame(parent)
{
	m_infoLabel = new QLabel;
	pGraphView = new StytleGraphView(this);
	pGraphView->setInfoLabel(m_infoLabel);
	pGraphView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pGraphView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pVBLayout = new QVBoxLayout;
	pVBLayout->setMargin(0);
	pVBLayout->setSpacing(0);
// 	QGraphicsView *pview = new QGraphicsView;
// 	pview->setViewport(pGraphView);
	pVBLayout->addWidget(pGraphView);
	pVBLayout->addWidget(m_infoLabel);
	setLayout(pVBLayout);
}

QTpGraphView::~QTpGraphView()
{
	if (pGraphView)
	{
		delete pGraphView;
		pGraphView = NULL;
	}
	if (pVBLayout)
	{
		delete pVBLayout;
		pVBLayout = NULL;
	}
}

void QTpGraphView::resizeEvent(QResizeEvent *e)
{
	QFrame::resizeEvent(e);
}

void QTpGraphView::setImg(QPixmap pImg)
{
	if (pGraphView)
	{
		pGraphView->setImg(pImg);
	}
}
