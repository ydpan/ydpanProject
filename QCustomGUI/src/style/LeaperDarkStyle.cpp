/*!
 * \file LeaperDarkStyle.cpp
 * \date 2018/02/28
 *
 * \author Lin, Chi
 * Contact: lin.chi@hzleaper.com
 *
 *
 * \note
*/

#include "LeaperDarkStyle.h"
#include <QPainter>
#include <QStyleOption>

LeaperDarkStyle::LeaperDarkStyle() : DarkStyle()
{

}

void LeaperDarkStyle::polish(QPalette &palette)
{
	DarkStyle::polish(palette);

	palette.setColor(QPalette::WindowText, QColor(215,215,215));
	palette.setColor(QPalette::ToolTipBase, QColor(215,215,215));
	palette.setColor(QPalette::Text, QColor(215,215,215));
	palette.setColor(QPalette::ButtonText, QColor(215,215,215));
	palette.setColor(QPalette::HighlightedText, QColor(215,215,215));

	palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(100, 100, 100));
	palette.setColor(QPalette::Disabled, QPalette::Text, QColor(100, 100, 100));
	palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(100, 100, 100));
	palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(0, 0, 0));
	palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(100, 100, 100));
}

void LeaperDarkStyle::polish(QApplication *app)
{
	if (!app) return;

	QFont defaultFont = QApplication::font();
	defaultFont.setPointSize(9);
	defaultFont.setFamily("Microsoft Yahei");
	app->setFont(defaultFont);

	// loadstylesheet
	QFile qfDarkstyle(QStringLiteral(":/lpdark/lpdarkstyle.qss"));
	if (qfDarkstyle.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		// set stylesheet
		QString qsStylesheet = QString::fromLatin1(qfDarkstyle.readAll());
		app->setStyleSheet(qsStylesheet);
		qfDarkstyle.close();
	}
}

void LeaperDarkStyle::drawControl(ControlElement element,
	const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
	if (element == QStyle::CE_ToolBoxTabShape) {
		// no painting.
	}
	else {
		DarkStyle::drawControl(element, option, painter, widget);
	}
}
