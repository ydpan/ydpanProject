#ifndef _CUSTOMGRAPHVIEW_H
#define _CUSTOMGRAPHVIEW_H

#include <QFrame>
#include "qgraphicsscene.h"
#include "qlabel.h"
#include "QVBoxLayout"
class QTpGraphView : public QFrame
{
	Q_OBJECT

public:
	QTpGraphView(QWidget *parent =0);
	~QTpGraphView();
	void setImg(QPixmap pImg);
protected:
	virtual void resizeEvent(QResizeEvent *e);
private:
	QLabel *m_infoLabel;
	class StytleGraphView *pGraphView;
	QGraphicsScene *pGrpScene;
	QVBoxLayout* pVBLayout;
};

#endif // QMYFRAME_H
