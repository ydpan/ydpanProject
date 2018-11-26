#ifndef __LeaperDarkStyle_h_
#define __LeaperDarkStyle_h_

#include "DarkStyle.h"

class  LeaperDarkStyle : public DarkStyle
{
	Q_OBJECT

public:
	LeaperDarkStyle();

	virtual void polish(QPalette &palette);
	virtual void polish(QApplication *app);

	virtual void drawControl(ControlElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = 0) const;
};

#endif // LeaperDarkStyle_h_

