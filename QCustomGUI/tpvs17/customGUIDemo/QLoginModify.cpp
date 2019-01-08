#include "QLoginModify.h"

QLoginModify::QLoginModify(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.button_change_ps, SIGNAL(clicked()), this, SLOT(onClicked()));
}

QLoginModify::~QLoginModify()
{

}

void QLoginModify::SetUserName(QString strName)
{
	ui.UserNamelineEdit->setText(strName);
}

Q_SLOT void QLoginModify::onClicked()
{

}
