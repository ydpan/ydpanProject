#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QAction>
#include <QPushButton>
class QSearchEdit : public QWidget
{
	Q_OBJECT

public:
	QSearchEdit(QWidget *parent = NULL);
	~QSearchEdit();
	QString text() {
		if (_pSearchLineEdit) {
			return _pSearchLineEdit->text();
		}
		return QString();
	};
signals:
	void onClicked();

private:
	QLineEdit *_pSearchLineEdit{nullptr};
	QPushButton *_pSearchButton{nullptr};
};
