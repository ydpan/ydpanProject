#include "QSqliteBase.h"

QSqliteBase::QSqliteBase(const QString& dbName, const QString& dbUser, const QString& dbPs)
	: QSqlDatabase("QSQLITE")
{
	setDatabaseName(dbName);
	if (!dbUser.isEmpty()){
		setUserName(dbUser);
		setPassword(dbPs);
	}
}

QSqliteBase::~QSqliteBase()
{

}

QString QSqliteBase::genInsertData(const QString & strTableName, const QVariantMap &vMap)
{
	QString strInsert = "INSERT INTO %1(%2) VALUES(%3)";
	QString strHeader;//
	QString strValue;// 

	QStringList strKeys = vMap.keys();
	for (int i = 0; i < strKeys.size(); i++) {
		strHeader +=  strKeys[i];
		if (i + 1 < strKeys.size())
			strHeader += ",";
		strValue += "'" + vMap[strKeys[i]].toString() + "'";
		if (i + 1 < strKeys.size())
			strValue += ",";
	}
	strInsert = strInsert.arg(strTableName, strHeader, strValue);
	return strInsert;
}

QString QSqliteBase::genSelect(const QString & strTableName, QStringList strSelectList, QString strClass)
{
	QString strDstSelect;
	if (strClass.isEmpty()) {
		strDstSelect = "SELECT %1 FROM %2";
	}
	else {
		strDstSelect = "SELECT %1 FROM %2 WHERE " + strClass;
	}

	if (0 == strSelectList.size()) {
		strDstSelect = strDstSelect.arg("*").arg(strTableName);
	}
	else {
		QString strSeletTmp = strSelectList[0];
		for (int i = 1; i < strSelectList.size(); i++) {
			strSeletTmp += "," + strSelectList[i];
		}
		strDstSelect = strDstSelect.arg(strSeletTmp, strTableName);
	}

	return strDstSelect;
}

/*生成 *=* 字符串*/
QString QSqliteBase::genClass(QString strName, QString strValue)
{
	QString strDst = "";
	strDst += strName + " = '" + strValue + "'";
	return strDst;
}

/*生成更新字符串*/
QString QSqliteBase::genUpdate(const QString & strTableName, QVariantMap &vMap, QString strClass)
{
	//#define _WF_SQL_UPDATE_INFO_MODEL   "UPDATE e_wh_models SET %1 = '%2' WHERE md_unique = '%3'"
	QString strDst = "";//
	if (strClass.isEmpty()) {
		strDst = "UPDATE %1 SET %2";//全部修改
	}
	else {
		strDst = "UPDATE %1 SET %2 WHERE %3";//指定修改
	}
	QString strUpdateTmp = "";
	if (vMap.size() != 0) {
		QStringList strList = vMap.keys();
		strUpdateTmp += genClass(strList[0], vMap[strList[0]].toString());
		for (int i = 1; i < strList.size(); i++) {
			strUpdateTmp += "," + genClass(strList[i], vMap[strList[i]].toString());
		}
	}

	strDst = strDst.arg(strTableName, strUpdateTmp, strClass);
	return strDst;
}

QString QSqliteBase::genDeleteData(const QString & strTableName, QString strClass)
{
	//#define _WF_SQL_DELETE_MODEL        "DELETE FROM e_wh_models WHERE md_unique = '%1'"
	QString strDst;
	if (strClass.isEmpty()) {
		strDst = "DELETE FROM %1";//全部删除
	}
	else {
		strDst = "DELETE FROM %1 WHERE %2";//指定删除
	}

	strDst = strDst.arg(strTableName, strClass);
	return strDst;
}

QString QSqliteBase::genInsertColumn(const QString & strTableName, QString strName, QString strType)
{
	QString strDst = "ALTER TABLE %1 ADD COLUMN %2 %3";
	strDst = strDst.arg(strTableName).arg(strName).arg(strType);

	return strDst;
}

QString QSqliteBase::genDeleteColumn(const QString & strTableName, QString strColumnName)
{
	QString strDst = "ALTER TABLE %1 DROP %2";
	strDst = strDst.arg(strTableName, strColumnName);
	return strDst;
}

QString QSqliteBase::genCreateTabel(const QString strTable, QString strPrimary, QString strType, QVariantMap &vMap)
{
	QString strDst = "CREATE TABLE IF NOT EXISTS %1 (%2)";
	QString strColumn = strPrimary + " " + strType;
	QStringList strList = vMap.keys();
	for (int i = 0; i < strList.size(); i++) {
		strColumn += ", " + strList[i] + " " + vMap[strList[i]].toString();
	}
	strDst = strDst.arg(strTable, strColumn);
	return strDst;
}
