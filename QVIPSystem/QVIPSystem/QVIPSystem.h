#ifndef _QVIPSYSTEM_H_
#define _QVIPSYSTEM_H_

#include <QtWidgets/QMainWindow>
#include "ui_QVIPSystem.h"
/*
����
1����Ա��Ϣ¼ȡ
2����Ա���۽��¼ȡ
3����Աɹ������¼ȡ
4����¼ÿ�����Աɹ�������۽��
5�����ݽ�ɹ��ͳ�Ƽ������
*/
class QVIPSystem : public QMainWindow
{
	Q_OBJECT

public:
	QVIPSystem(QWidget *parent = Q_NULLPTR);

private:
	Ui::QVIPSystemClass ui;
};
#endif
