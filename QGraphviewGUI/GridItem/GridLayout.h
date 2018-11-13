#ifndef _GRIDLAYOUT_H_
#define _GRIDLAYOUT_H_

#include <QGraphicsItem>
#include <QColor>
#include <QFrame>
#include <QGraphicsView>

#define GRID_LAYOUT_VIEW_MIN_SCALE 0
#define GRID_LAYOUT_VIEW_MAX_SCALE 500
#define GRID_LAYOUT_CHIP_MARGIN 2
#define GRID_LAYOUT_CHIP_W  15
#define GRID_LAYOUT_CHIP_H  15

enum emGridItemResult {//0用于表示没有检测位置 1及以上数字表示有检测工位
	GRID_NO_DETECT = 1,
	GRID_DETECTED_NG,
	GRID_DETECTED_A,
	GRID_DETECTED_B,
	GRID_DETECTED_NULL,
};

class CGridChipItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CGridChipItem(int index, QWidget *parent = 0);

    QRectF boundingRect() const override;

    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, 
                QWidget *widget) override;

    void resetDOE();
    void setStatus(emGridItemResult dtRslt);
	const emGridItemResult getStatus() { return m_detectRslt;}

	const QColor& GetColor() { return m_color; }

	void setRowCol(int nRow, int nCol);
	int rowIndex() { return m_row; }
	int colIndex() { return m_col; }

	int getRealIndex() { return m_index; }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:    
	void sgnChipClicked(int, int);
private:
    const static QColor COLOR_INIT; // = QColor(170, 170, 170);
    const static QColor COLOR_CLASS_A; // = QColor(85, 255, 127);
	const static QColor COLOR_CLASS_B;// = QColor(85, 255, 250);
    const static QColor COLOR_CLASS_NULL; // = QColor(255, 255, 255);
    const static QColor COLOR_CLASS_NG; // = QColor(170, 0, 0);
    QColor m_color;

    emGridItemResult m_detectRslt{ GRID_NO_DETECT };
    float m_flawProportion{ 0.0 };
    int m_index{ 0 };
	int m_row{ 0 };
	int m_col{ 0 };
};

class CGridLayoutView : public QGraphicsView
{
    Q_OBJECT
public:
    CGridLayoutView(QWidget *parent = 0);

	int GetScale() { return m_scale; }
	void SetScale(int scale) { m_scale = scale; setupMatrix(); }

protected:
    void wheelEvent(QWheelEvent *) override;

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void setupMatrix();
    void resetView();
    void print();
private:
    int m_scale;
};
#endif