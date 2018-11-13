#include <QFile>
#include <QDebug>
#include "GridTray.h"

CGridTray::CGridTray(QObject* parent /* = 0 */)
    : QObject(parent)
{
    m_trayRoadmapLayout.clear();
}

CGridTray::~CGridTray()
{

}

void CGridTray::createMapfromfile(const QString& filepath)
{
	m_trayRoadmapLayout.clear();

	m_validDOECnt = 0;
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Failed to load tray layout file:" << filepath << __FUNCTION__;
		return;
	}

	int cols = 0;
	auto bad_layout_file = [&](void) {
		qWarning() << "Bad layout file:" << filepath << __FUNCTION__;
		this->m_trayRoadmapLayout.clear();
	};


	int m_nRow = 0;
	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		QStringList linelist = QString(line).split(";");

		if (linelist.empty()) break;
		linelist.removeAll(" ");
		linelist.removeAll("\n");
		linelist.removeAll("");
		linelist.removeAll("\r");
		if (linelist.size() <= 0)
			continue;
		if (0 == cols) {
			cols = linelist.size();
		}
		else if (cols != linelist.size()) {
			bad_layout_file();
			return;
		}
		int m_nCol = 0;
		t_vecGrid rowDoes;
		foreach(const QString& s, linelist) {
			bool bok = false;
			int nval = s.toInt(&bok);
			if (!bok) {
				bad_layout_file();
				return;
			}

			t_spGrid pDoe = nullptr;
			if (nval > 0) {
				pDoe = QSharedPointer<CGridChipItem>(new CGridChipItem(m_validDOECnt));
				pDoe->setRowCol(m_nRow, m_nCol);
				pDoe->setStatus(emGridItemResult(nval));
				connect(pDoe.data(), SIGNAL(sgnChipClicked(int, int)), this, SIGNAL(sigChipSelected(int, int)));
				m_validDOECnt++;
			}
			rowDoes.append(pDoe);
			m_nCol++;
		}

		m_trayRoadmapLayout.append(rowDoes);
		m_nRow++;
	}
	file.close();
}

void CGridTray::buildRoadmapFromLayoutFile(const QString& pathLayout)
{
    m_trayRoadmapLayout.clear();

    m_validDOECnt = 0;
    QFile file(pathLayout);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to load tray layout file:" << pathLayout << __FUNCTION__;
        return;
    }

    int cols = 0;
    auto bad_layout_file = [&](void) {
        qWarning() << "Bad layout file:" << pathLayout << __FUNCTION__;
        this->m_trayRoadmapLayout.clear();
    };

	
	int m_nRow = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QStringList linelist = QString(line).split(" ");
        
        if(linelist.empty()) break;
		linelist.removeAll(" ");
		linelist.removeAll("\n");
		linelist.removeAll("");
		linelist.removeAll("\r");
		if(linelist.size()<=0)
			continue;
        if (0 == cols) {
            cols = linelist.size();
        }
        else if (cols != linelist.size()) {
            bad_layout_file();
			file.close();
            return;
        }
		int m_nCol = 0;
        t_vecGrid rowDoes;
        foreach(const QString& s, linelist) {
            bool bok = false;
            int nval = s.toInt(&bok);
            if (!bok) {
                bad_layout_file();
				file.close();
                return;
            }

            t_spGrid pDoe = nullptr;
            if (1 == nval) {
                pDoe = QSharedPointer<CGridChipItem>(new CGridChipItem(m_validDOECnt));
				pDoe->setRowCol(m_nRow, m_nCol);
                connect(pDoe.data(), SIGNAL(sgnChipClicked(int, int)), this, SIGNAL(sigChipSelected(int, int)));
                m_validDOECnt++;
            }
            rowDoes.append(pDoe);
			m_nCol++;
        }

        m_trayRoadmapLayout.append(rowDoes);
		m_nRow++;
    }
	file.close();
}

int CGridTray::getRows() const
{
    return m_trayRoadmapLayout.size();
}

int CGridTray::getCols() const
{
    if (m_trayRoadmapLayout.empty()) {
        return 0;
    }

    return m_trayRoadmapLayout.at(0).size();
}

t_spGrid CGridTray::getEntry(int row, int col)
{
	if (row < 0 || row >= m_trayRoadmapLayout.size())
		return t_spGrid();
	if (col < 0 || col >= m_trayRoadmapLayout.at(0).size())
		return t_spGrid();

    Q_ASSERT((row >= 0) && (row < m_trayRoadmapLayout.size()));
    Q_ASSERT((col >= 0) && (col < m_trayRoadmapLayout.at(0).size()));

    return m_trayRoadmapLayout.at(row).at(col);
}

int CGridTray::GetMostCommonCol(bool bRight)
{
	if (bRight)
	{
		auto vecFirst = m_trayRoadmapLayout.first();
		auto vecLast = m_trayRoadmapLayout.last();
		for (int i = vecLast.size() - 1; i >= 0; i--)
		{
			auto doeL = vecLast.at(i);
			if (!doeL.isNull())
			{
				auto doeF = vecFirst.at(i);
				if (!doeF.isNull())
				{
					return i;
				}
			}
		}
	}
	else
	{
		auto vecFirst = m_trayRoadmapLayout.first();
		auto vecLast = m_trayRoadmapLayout.last();
		for (int i = 0; i < vecLast.size(); i++)
		{
			auto doeL = vecLast.at(i);
			if (!doeL.isNull())
			{
				auto doeF = vecFirst.at(i);
				if (!doeF.isNull())
				{
					return i;
				}
			}
		}
	}

	return -1;
}

t_spGrid CGridTray::getFirstByOrder()
{
	static t_spGrid s_doeFirst = nullptr;
	if (s_doeFirst.isNull())
	{
		foreach(auto vecDoe, m_trayRoadmapLayout) {
			foreach(auto pDoe, vecDoe) {
				if (!pDoe.isNull()) {
					s_doeFirst = pDoe;	
					return s_doeFirst;
				}
			}
		}
	}
	
	return s_doeFirst;
}

t_spGrid CGridTray::getLastByOrder()
{
	static t_spGrid s_doeLast = nullptr;
	if (s_doeLast.isNull())
	{
		for (int i = m_trayRoadmapLayout.size() - 1; i >= 0; i--)
		{
			auto vec = m_trayRoadmapLayout.at(i);
			for (size_t j = vec.size() - 1; j >= 0; j--)
			{
				auto doe = vec.at(j);
				if (!doe.isNull())
				{
					s_doeLast = doe;
					return s_doeLast;
				}
			}
		}
	}

	return s_doeLast;
}

int CGridTray::getOrderIndex(int row, int col)
{
	auto doe = getEntry(row, col);
	if (!doe.isNull())
	{
		return doe->getRealIndex();
	}
	return -1;
}

void CGridTray::resetAllEntry()
{
    foreach(t_vecGrid vecDoe, m_trayRoadmapLayout) {
        foreach(t_spGrid pDoe, vecDoe) {
            if (!pDoe.isNull()) {
                pDoe->resetDOE();
            }
        }
    }
}

CGridProject::CGridProject(QObject *parent /*= 0*/)
{
	_pScene = new QGraphicsScene();		
	_spGridTray = QSharedPointer<CGridTray>(new CGridTray);
}

CGridProject::~CGridProject()
{

}

bool CGridProject::LoadAndInit(QString strpath)
{
	if (!_spGridTray)
		return false;
	if (!_pScene)
		return false;
	_spGridTray->buildRoadmapFromLayoutFile(strpath);
	connect(_spGridTray.data(), SIGNAL(sigChipSelected(int, int)), this, SIGNAL(sigItemClicked(int, int)));
	int nRows = _spGridTray->getRows();
	int nCols = _spGridTray->getCols();

	_pScene->clear();
	
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			t_spGrid gridItem = _spGridTray->getEntry(i, j);
			if (!gridItem.isNull()) {
				QGraphicsItem *pItem = gridItem.data();
				pItem->setPos(j*GRID_LAYOUT_CHIP_H, i*GRID_LAYOUT_CHIP_W);
				_pScene->addItem(pItem);
			}
		}
	}
	return true;
}

void CGridProject::updateItem(int row, int col, emGridItemResult rlt/*= GRID_NO_DETECT*/)
{
	t_spGrid tGridItem = _spGridTray->getEntry(row, col);
	if (!tGridItem.isNull()) {
		tGridItem->setStatus(rlt);
		_pScene->update(col*GRID_LAYOUT_CHIP_H, row*GRID_LAYOUT_CHIP_W, GRID_LAYOUT_CHIP_W, GRID_LAYOUT_CHIP_H);
	}
}

emGridItemResult CGridProject::getItemStatus(int row, int col) const
{
	t_spGrid tGridItem = _spGridTray->getEntry(row, col);
	if (!tGridItem.isNull()) {
		return tGridItem->getStatus();
	}
	return GRID_DETECTED_NULL;
}

bool CGridProject::IsEmptyItem(int row, int col) const
{
	t_spGrid tGridItem = _spGridTray->getEntry(row, col);
	if (!tGridItem.isNull()) {
		return true;
	}
	return false;
}

