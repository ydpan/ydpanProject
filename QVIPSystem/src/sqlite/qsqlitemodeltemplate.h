#ifndef QSQLITEMODELTEMPLATE_H
#define QSQLITEMODELTEMPLATE_H

#include "QSqliteVipSystemDB.h"
class QSqliteModelTemplate : public QSqliteVipSystemDB
{
public:
	QSqliteModelTemplate(const QString& dbPath);
	~QSqliteModelTemplate();
	virtual bool InitDatabase();
	virtual bool addData(QVariantMap m_map);
	//bool ReadOutData(QMap<QString, class IWheelModel*> &m_MapString);//读出所有数据
	bool DeleteModelData(QString m_modelName);//删除一条数据
	bool UpdateModelData(QString m_modelName, QVariantMap m_map);//更新一条数据
private:
	
};

#endif // QSQLITEMODELTEMPLATE_H
