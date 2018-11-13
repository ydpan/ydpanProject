#include <QtWidgets>
#include "GridLayout.h"

const QColor CGridChipItem::COLOR_INIT = QColor(170, 170, 170);
const QColor CGridChipItem::COLOR_CLASS_A = QColor(0, 255, 0);
const QColor CGridChipItem::COLOR_CLASS_B = QColor(255, 255, 0);
const QColor CGridChipItem::COLOR_CLASS_NULL = QColor(255, 255, 255);
const QColor CGridChipItem::COLOR_CLASS_NG = QColor(255, 0, 0);

CGridChipItem::CGridChipItem(int index, QWidget *parent)
    : QObject(parent)
    , m_index(index)
    , m_color(COLOR_INIT)
{
    setFlags(ItemIsSelectable);
}

QRectF CGridChipItem::boundingRect() const
{
    return QRectF(0, 0, GRID_LAYOUT_CHIP_W, GRID_LAYOUT_CHIP_H);
}

void CGridChipItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, 
                      QWidget *widget)
{    
    painter->setBrush(QBrush(m_color));

    painter->drawRect(QRect(GRID_LAYOUT_CHIP_MARGIN, GRID_LAYOUT_CHIP_MARGIN, 
                            GRID_LAYOUT_CHIP_W - 2* GRID_LAYOUT_CHIP_MARGIN, 
                            GRID_LAYOUT_CHIP_H - 2* GRID_LAYOUT_CHIP_MARGIN));
}

void CGridChipItem::resetDOE()
{
    m_flawProportion = 0.0;
    m_detectRslt = GRID_NO_DETECT;
    m_color = COLOR_INIT;
}

void CGridChipItem::setStatus(emGridItemResult dtRslt)
{
    m_detectRslt = dtRslt;
    
    if (m_detectRslt == GRID_NO_DETECT) {
        m_color = COLOR_INIT;
    }
    else if (m_detectRslt == GRID_DETECTED_NG) {
        m_color = COLOR_CLASS_NG;
    }
    else if (m_detectRslt == GRID_DETECTED_A) {
        m_color = COLOR_CLASS_A;
    }
	else if (m_detectRslt == GRID_DETECTED_B) {
		m_color = COLOR_CLASS_B;
	}
    else if (m_detectRslt == GRID_DETECTED_NULL) {
        m_color = COLOR_CLASS_NULL;
    }
}

void CGridChipItem::setRowCol(int nRow, int nCol)
{
	m_row = nRow;
	m_col = nCol;
}

void CGridChipItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
	emit sgnChipClicked(m_row, m_col);
}

//////////////////////////////////////////////////////////
CGridLayoutView::CGridLayoutView(QWidget *parent /* = 0 */)
    : QGraphicsView(parent)
    , m_scale(GRID_LAYOUT_VIEW_MAX_SCALE/2)
{
    //this->setRenderHint(QPainter::Antialiasing, false);
    //this->setDragMode(QGraphicsView::RubberBandDrag);
    //this->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    //this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    //this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

}

void CGridLayoutView::resetView()
{

}

void CGridLayoutView::print()
{
    //QPrinter printer;
    //QPrintDialog dialog(&printer, this);
    //if (dialog.exec() == QDialog::Accepted) {
    //    QPainter painter(&printer);
    //    graphicsView->render(&painter);
    //}
}

void CGridLayoutView::zoomIn(int level /* = 1 */)
{
    m_scale = qMin(m_scale + level, GRID_LAYOUT_VIEW_MAX_SCALE);

    setupMatrix();
}

void CGridLayoutView::zoomOut(int level /* = 1 */)
{
    m_scale = qMax(m_scale - level, GRID_LAYOUT_VIEW_MIN_SCALE);
    setupMatrix();
}

void CGridLayoutView::setupMatrix()
{
    qreal scale = qPow(qreal(2), (m_scale - GRID_LAYOUT_VIEW_MAX_SCALE / 2) / qreal(50));
    QMatrix matrix;
    matrix.scale(scale, scale);

    this->setMatrix(matrix);
}

void CGridLayoutView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            zoomIn(6);
        else
            zoomOut(6);
        e->accept();
    }
    else {
        QGraphicsView::wheelEvent(e);
    }
}
