#include "qsqlitemodeltemplate.h"

QSqliteModelTemplate::QSqliteModelTemplate(const QString& dbPath)
	: QSqliteVipSystemDB(dbPath)
{

}

QSqliteModelTemplate::~QSqliteModelTemplate()
{

}

bool QSqliteModelTemplate::InitDatabase()
{
	if (!isOpen() && !open())
	{
		return false;
	}
	QVariantMap vMap;
	vMap.insert(_MD_MODEL, "VARCHAR(32)");//model 模型名称
	vMap.insert(_MD_DIAMETER, "INT(100)");//直径
	vMap.insert(_MD_HIGHT, "INT(100)");//高度
	vMap.insert("picpath", "VARCHAR(32)");
	vMap.insert(_MD_ROTATE, "INT(100)");
	if (!CreateTables("modelTable", _MD_PRIMARY_KEY, vMap))
	{
		return false;
	}
	return true;
}

bool QSqliteModelTemplate::addData(QVariantMap m_map)
{
	return InsertOneData("modelTable", m_map);
}
// bool QSqliteModelTemplate::ReadOutData(QMap<QString, IWheelModel*> &m_MapString)
// {
// 	QString select_sql = QString("select * from ") + "modelTable";
// 	QSqlQuery sql = exec(select_sql);
// 	QSqlError err = sql.lastError();
// 	int t = err.type();
// 	if (t != QSqlError::NoError)
// 	{
// 		return false;
// 	}
// 	m_MapString.clear();
// 	while (sql.next())
// 	{
// 		WheelModel * pModel = new WheelModel;
// 		pModel->m_strModelID = sql.value(_MD_MODEL).toString();
// 		pModel->m_dDiameter = sql.value(_MD_DIAMETER).toDouble();
// 		pModel->m_dHeight = sql.value(_MD_HIGHT).toDouble();
// 		pModel->m_strPath = sql.value("picpath").toString();
// 		pModel->m_nRotate = sql.value(_MD_ROTATE).toInt();
// 		m_MapString.insert(pModel->m_strModelID, pModel);
// 	}
// 	return true;
// }

bool QSqliteModelTemplate::DeleteModelData(QString m_modelName)
{
	QString m_map = genClass(_MD_MODEL, m_modelName);//!>生成where条件语句
	QString strSql = genDeleteData("modelTable", m_map);
	exec(strSql);
	if (lastError().isValid())
	{
		return false;
	}
	return true;
}

bool QSqliteModelTemplate::UpdateModelData(QString m_modelName, QVariantMap m_map)
{
	if (m_modelName.isEmpty() || m_map.empty()) {
		return false;
	}
	QString m_str = genClass(_MD_MODEL, m_modelName);//!>生成where条件语句
	QString strSql = genUpdate("modelTable", m_map,m_str);
	exec(strSql);
	if (lastError().isValid())
	{
		return false;
	}
	return true;
}
