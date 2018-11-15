/*!
 * \file LeaperDarkStyle.h
 * \date 2018/02/28
 *
 * \author Lin, Chi
 * Contact: lin.chi@hzleaper.com
 *
 *
 * \note
*/

#ifndef __LeaperDarkStyle_h_
#define __LeaperDarkStyle_h_

#include "../3rdParty/Qt-Frameless-Window-DarkStyle/DarkStyle.h"
#include "AngelaCommon.h"

class ANGELA_DLLSPEC LeaperDarkStyle : public DarkStyle
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

