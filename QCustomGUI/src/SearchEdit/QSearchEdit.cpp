#include "QSearchEdit.h"
#include <QPushButton>
#include <QHBoxLayout>
QSearchEdit::QSearchEdit(QWidget *parent)
	: QWidget(parent)
{
	_pSearchLineEdit = new QLineEdit();
	_pSearchButton = new QPushButton(this);
	_pSearchLineEdit->setMinimumHeight(50);
	_pSearchButton->setCursor(Qt::PointingHandCursor);
	_pSearchButton->setFixedSize(100, 50);
	_pSearchButton->setToolTip(QString::fromLocal8Bit("ËÑË÷"));
	//_pSearchButton->setStyleSheet("QPushButton{border-image:url(:/Resources/app8.png); background:transparent;} \
                                     QPushButton:hover{border-image:url(:/Resources/app8.png)} \
                                     QPushButton:pressed{border-image:url(:/Resources/app8.png)}");

	QMargins margins = _pSearchLineEdit->textMargins();
	_pSearchLineEdit->setTextMargins(margins.left(), margins.top(), _pSearchLineEdit->width(), margins.bottom());
	_pSearchLineEdit->setPlaceholderText(QString::fromLocal8Bit("place input search things"));
	QHBoxLayout *pSearchLayout = new QHBoxLayout();
	pSearchLayout->addStretch();
	pSearchLayout->addWidget(_pSearchButton);
	pSearchLayout->setSpacing(0);
	pSearchLayout->setContentsMargins(0, 0, 0, 0);
	_pSearchLineEdit->setLayout(pSearchLayout);
	connect(_pSearchButton, SIGNAL(clicked()), this, SIGNAL(onClicked()));
	_pSearchButton->show();
	QHBoxLayout *playout = new QHBoxLayout;
	playout->addWidget(_pSearchLineEdit);
	this->setLayout(playout);
}

QSearchEdit::~QSearchEdit()
{
}
