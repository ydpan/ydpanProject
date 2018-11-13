#ifndef _GRIDTRAY_H_
#define _GRIDTRAY_H_

#include <QVector>
#include <QObject>
#include <QSharedPointer>
#include <QGraphicsScene>
#include "GridLayout.h"

typedef QSharedPointer<CGridChipItem> t_spGrid;
typedef QVector< t_spGrid > t_vecGrid;

class CGridTray : public QObject
{
    Q_OBJECT
public:
    CGridTray(QObject* parent=0);
    ~CGridTray();
	void createMapfromfile(const QString& filepath);
    void buildRoadmapFromLayoutFile(const QString& pathLayout);
    int getRows() const;
    int getCols() const;
    t_spGrid getEntry(int row, int col);

	int firstDoeRow() { return getFirstByOrder()->rowIndex(); }
	int firstDoeCol() { return getFirstByOrder()->colIndex(); }

	//×î×óÓÒ¹«ÁÐ
	int GetMostCommonCol(bool bRight);

	t_spGrid getFirstByOrder();
	t_spGrid getLastByOrder();

	int getOrderIndex(int row, int col);

    void resetAllEntry();

    int getValidDOECount() const { return m_validDOECnt; }

signals:    
	void sigChipSelected(int, int);

private:
    QVector< t_vecGrid > m_trayRoadmapLayout;

    int m_validDOECnt{ 0 };

};

class CGridProject :public QObject
{
	Q_OBJECT
public:
	CGridProject(QObject *parent = 0);
	~CGridProject();
	QGraphicsScene * getScenePtr()const { return _pScene; };
	bool LoadAndInit(QString strpath);
	int getRows() const { return _spGridTray->getRows(); };
	int getCols() const { return _spGridTray->getCols(); };
	void updateItem(int row ,int col, emGridItemResult rlt= GRID_NO_DETECT);
	emGridItemResult getItemStatus(int row,int col) const;
	bool IsEmptyItem(int row,int col) const;
signals:
	void sigItemClicked(int row,int col);
private:
	QGraphicsScene * _pScene{ nullptr };
	QSharedPointer<CGridTray> _spGridTray;
};

#endif
