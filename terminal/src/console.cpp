#include "console.h"
#include <QScrollBar>
#include <QtCore/QDebug>
#pragma execution_character_set("utf-8")
Console::Console(QWidget *parent)
    : QPlainTextEdit(parent)
    , localEchoEnabled(false)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}

void Console::putData(const QByteArray &data)
{
	QString strShow = QString(data);
	strShow.remove("\r");

	if (strShow==QString("\b \b"))
	{
		QTextCursor textCursor = this->textCursor();
 		textCursor.movePosition(QTextCursor::End);
		textCursor.deletePreviousChar();
	}
	else if (strShow==QString("\x1b[H\x1b[2J"))
	{
		clear();
	}
	else if (strShow == "\a")
	{
		QTextCursor textCursor = this->textCursor();
		textCursor.movePosition(QTextCursor::End);
		textCursor.select(QTextCursor::BlockUnderCursor);
		QString str = textCursor.selectedText();
		str.remove("''");
		str.remove(" ");
		if (str.size()<=2&&str.contains("#"))
		{
			int a = 0;
		}
		else 
		{
			textCursor.movePosition(QTextCursor::End);
			textCursor.deletePreviousChar();
		}
 			
	}
	else if (strShow.contains("\b \b"))
	{
		QTextCursor textCursor = this->textCursor();
		textCursor.movePosition(QTextCursor::End);
		textCursor.deletePreviousChar();
		strShow.remove("\b \b");
		insertPlainText(strShow);
	}
	else
	{
		strShow.remove(" \b");
		insertPlainText(strShow);
	}
	
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void Console::keyPressEvent(QKeyEvent *e)
{
	//int key = e->key();
    switch (e->key()) {
//     case Qt::Key_Backspace:
// 	{
// 		if(m_cmdList.size()>0)
// 			m_cmdList.removeLast();
// 		break;
// 	}
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        emit getData(e->text().toLatin1());
// 		if (localEchoEnabled)
//             QPlainTextEdit::keyPressEvent(e);
		break;
    }
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
