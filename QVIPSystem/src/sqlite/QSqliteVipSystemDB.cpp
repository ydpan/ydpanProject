#include "QSqliteVipSystemDB.h"
#include "qmutex.h"

QSqliteVipSystemDB::QSqliteVipSystemDB(const QString& dbPath)
	: QSqliteBase(dbPath), m_mxLock(QMutex::Recursive)
{

}

QSqliteVipSystemDB::~QSqliteVipSystemDB()
{

}

bool QSqliteVipSystemDB::InitDatabase()
{
	if (!isOpen() && !open())
	{
		return false;
	}

	QVariantMap vMap;//vip 人员信息记录表
	vMap.insert("vipiD", "INT(100)");//VIP编号
	vMap.insert("strName", "VARCHAR(32)");//成员名称
	vMap.insert("phoneNum", "VARCHAR(32)");//电话号码
	vMap.insert("chartName", "VARCHAR(32)");//微信名
	vMap.insert("address", "VARCHAR(32)");//地址
	vMap.insert("about", "VARCHAR(32)");
	vMap.insert("reserve1", "VARCHAR(32)");
	vMap.insert("reserve2", "VARCHAR(32)");
	vMap.insert("reserve3", "VARCHAR(32)");
	vMap.insert("time", "DATETIME");
	CreateTables(_TABLE_USERINFO_TABLE_, _PRIMARY_KEY_, vMap);


	vMap.clear();//总数据统计表
	vMap.insert("vipiD", "INT(100)");//VIP编号
	vMap.insert("monetary", "REAL(100)");
	vMap.insert("Drying_list", "INT(100)");
	vMap.insert("Integration", "REAL(100)");
	vMap.insert("time", "DATETIME");//更新时间
	CreateTables(_TABLE_TOTAL_TABLE_, _MD_PRIMARY_KEY, vMap);
	
	vMap.clear();//消费记录表 记录每天消费的金额
	vMap.insert("vipiD", "INT(100)");//VIP编号
	vMap.insert("monetary", "REAL(100)");
	vMap.insert("time", "DATETIME");//消费时间
	vMap.insert("updatetime", "DATETIME");//记录时间
	CreateTables(_TABLE_MOMEY_TABLE_, _MD_PRIMARY_KEY, vMap);

	vMap.clear();//晒单数据记录表
	vMap.insert("vipiD", "INT(100)");//VIP编号
	vMap.insert("drylist", "REAL(100)");
	vMap.insert("time", "DATETIME");
	vMap.insert("updatetime", "DATETIME");
	CreateTables(_TABLE_DRY_TABLE_, _MD_PRIMARY_KEY, vMap);

	vMap.clear();//积分消费记录
	vMap.insert("vipiD", "INT(100)");//VIP编号
	vMap.insert("Integration", "REAL(100)");
	vMap.insert("about", "VARCHAR(100)");
	vMap.insert("time", "DATETIME");
	CreateTables(_TABLE_SUBINTEGRATION_TABLE_, _MD_PRIMARY_KEY, vMap);
	return true;
}

bool QSqliteVipSystemDB::CreateTables(QString m_tableName, QString m_primary, QVariantMap m_map)
{
	QString strSql = genCreateTabel(m_tableName, m_primary, "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL", m_map);
	QSqlQuery sql = exec(strSql);
	QSqlError err = sql.lastError();
	int t = err.type();
	if (t != QSqlError::NoError)
	{
		return false;
	}
	return true;
}

void QSqliteVipSystemDB::FreeDatabase()
{
	close();
}


bool QSqliteVipSystemDB::AddNewVipMemberInfo(VipMemberInfo &info)
{
	QVariantMap m_map;
	m_map.insert("vipiD", info.nVipNum);//VIP编号
	m_map.insert("strName", info.strName);//成员名称
	m_map.insert("phoneNum", info.nPhoneNumber);//电话号码
	m_map.insert("chartName", info.strWeixinName);//微信名
	m_map.insert("address", info.strAddress);//地址
	m_map.insert("about", info.strAbout);
	m_map.insert("reserve1", info.reserve1);
	m_map.insert("reserve2", info.reserve2);
	m_map.insert("reserve3", info.reserve3);
	QString strData = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
	m_map.insert("time", strData);

	QString strInsert = "INSERT INTO %1(%2) VALUES(%3)";
	QString strHeader;//
	QString strValue;// 

	QStringList strKeys = m_map.keys();
	for (int i = 0; i < strKeys.size(); i++) {
		strHeader += strKeys[i];
		if (i + 1 < strKeys.size())
			strHeader += ",";
		strValue += ":" + strKeys[i] + "";

		if (i + 1 < strKeys.size())
			strValue += ",";
	}
	strInsert = strInsert.arg(_TABLE_USERINFO_TABLE_, strHeader, strValue);
	QSqlQuery query = exec();
	query.prepare(strInsert);
	for (int i = 0; i < strKeys.size(); i++)
	{
		QString m_keys = strKeys.at(i);
		QString str = ":" + m_keys;
		query.bindValue(str, m_map.value(m_keys));
	}
	query.exec();
	return true;
}


int QSqliteVipSystemDB::getVipMemberMaxID()
{
	int uId = 0; 
	QString select_max_sql = QString("SELECT * FROM %1 ORDER BY %2 DESC;").arg(_TABLE_USERINFO_TABLE_).arg("vipiD");;
	QSqlQuery sql = exec(select_max_sql);
	int max = 0;
	while (sql.next())
	{
		uId = sql.value(0).toInt();
		if (max < uId)
			max = uId;
	}
	max += 1;
	return max;
}

// bool QSqliteApplication::ReadOutTimeData(QMap<QString, TimeStruct> &m_MapTimeMatch)
// {
// 	QString select_sql = QString("select * from ") + _MD_TIMETABLE;
// 	QSqlQuery sql = exec(select_sql);
// 	QSqlError err = sql.lastError();
// 	int t = err.type();
// 	if (t != QSqlError::NoError)
// 	{
// 		return false;
// 	}
// 	while (sql.next())
// 	{
// 		int nIndex = sql.value("uid").toInt();
// 		QString type = sql.value(_MD_TYPE).toString();
// 		QTime start = sql.value(_MD_STARTTIME).toTime();
// 		QTime end = sql.value(_MD_ENDTIME).toTime();
// // 		TimeStruct m_timeStruct;
// // 		m_timeStruct.m_Index = nIndex;
// // 		m_timeStruct.m_name = type;
// // 		m_timeStruct.m_startTime = start;
// // 		m_timeStruct.m_endTime = end;
// // 		m_MapTimeMatch.insert(type, m_timeStruct);
// 	}
// 	return true;
// }
// bool QSqliteApplication::AddOneWarningMessage(WarnMessage m_messageInfo)
// {
// 	QVariantMap m_map;
// 	m_map.insert(_MD_MESSAGE, m_messageInfo.m_Message);
// 	m_map.insert(_MD_UPDATE_TIME, m_messageInfo.m_Date);
// 	m_map.insert(_MD_CLASS, m_messageInfo.m_class);
// 	return InsertOneData(_MD_WARNINGTABLE, m_map);
// }
// bool QSqliteApplication::AddOneTime(TimeStruct &m_timestruct)
// {
// 	QVariantMap m_map;
// // 	m_map.insert(_MD_TYPE, m_timestruct.m_name);
// // 	m_map.insert(_MD_STARTTIME, m_timestruct.m_startTime);
// // 	m_map.insert(_MD_ENDTIME, m_timestruct.m_endTime);
// 	return InsertOneData(_MD_TIMETABLE, m_map);
// }

// bool QSqliteApplication::DelOneTime(TimeStruct &m_timestruct)
// {
// // 	QString m_map = genClass(_MD_TYPE,m_timestruct.m_name);//!>生成where条件语句
// // 	QString strSql = genDeleteData(_MD_TIMETABLE, m_map);
// // 	exec(strSql);
// // 	if (lastError().isValid())
// // 	{
// // 		return false;
// // 	}
// 	return true;
// }

bool QSqliteVipSystemDB::ReadOutTimeData(QMap<QString, QMap<QString, QTime>> &m_MapString)
{
	QString select_sql = QString("select * from ") + _MD_TIMETABLE;
	QSqlQuery sql = exec(select_sql);
	QSqlError err = sql.lastError();
	int t = err.type();
	if (t != QSqlError::NoError)
	{
		return false;
	}
	while (sql.next())
	{
		QString type = sql.value(_MD_TYPE).toString();
		QTime start =sql.value(_MD_STARTTIME).toTime();
		QTime end = sql.value(_MD_ENDTIME).toTime();
		m_MapString.insert(type, QMap<QString, QTime>());
		m_MapString[type].insert(_MD_STARTTIME, start);
		m_MapString[type].insert(_MD_ENDTIME, end);
	}

	return true;
}

bool QSqliteVipSystemDB::UpdateTimeData(QMap<QString, QMap<QString, QTime>> m)
{
	QVariantMap m_insertmap;
	for (QMap<QString, QMap<QString, QTime>>::iterator its = m.begin(); its != m.end(); ++its)
	{	
		QString type = its.key();
		
		{
			m_insertmap.insert(_MD_TYPE, QVariant(type));
			m_insertmap.insert(_MD_STARTTIME, QVariant(m[type][_MD_STARTTIME].toString("hh:mm:ss")));
			m_insertmap.insert(_MD_ENDTIME, QVariant(m[type][_MD_ENDTIME].toString("hh:mm:ss")));
			InsertOneData(_MD_TIMETABLE, m_insertmap); 
		}


		QString update_sql = QString("update ") + _MD_TIMETABLE\
			+ QString(" set %1 = '%2',%3 = '%4' where ").arg(_MD_STARTTIME)\
			.arg(m[type][_MD_STARTTIME].toString("hh:mm:ss")).arg(_MD_ENDTIME)\
			.arg(m[its.key()][_MD_ENDTIME].toString("hh:mm:ss"))\
			+ _MD_TYPE + QString(" = '%1'").arg(its.key());
		QSqlQuery sql = exec(update_sql);
		QSqlError err = sql.lastError();
		int t = err.type();
		if (t != QSqlError::NoError)
		{
			return false;
		}

	}
	return true;
}
bool QSqliteVipSystemDB::addData(QVariantMap m_map)
{
	return InsertOneData(_MD_TABLE_FORMS_MANE, m_map);
}

bool QSqliteVipSystemDB::AddOneData(QVariantMap m_map)
{
	QString strInsert = "INSERT INTO %1(%2) VALUES(%3)";
	QString strHeader;//
	QString strValue;// 

	QStringList strKeys = m_map.keys();
	for (int i = 0; i < strKeys.size(); i++) {
		strHeader += strKeys[i];
		if (i + 1 < strKeys.size())
			strHeader += ",";
		strValue += ":" + strKeys[i] + "";

		if (i + 1 < strKeys.size())
			strValue += ",";
	}
	strInsert = strInsert.arg(_MD_TABLE_FORMS_MANE, strHeader, strValue);
	//return strInsert;
	QSqlQuery query =exec();
	query.prepare(strInsert);
 	for (int i = 0; i < strKeys.size(); i++)
	{
		QString m_keys = strKeys.at(i);
		QString str = ":" + m_keys;
		query.bindValue(str, m_map.value(m_keys));
 	}
	query.exec();
	return true;
}

bool QSqliteVipSystemDB::InsertOneData(QString m_tableName, QVariantMap m_map)
{
	if (m_tableName.isEmpty() || m_map.empty()) {
		return false;
	}
	QString strSql;
	strSql = genInsertData(m_tableName, m_map);
	exec(strSql);
	if (lastError().isValid())
	{
		return false;
	}
	return true;
}

//test code
bool QSqliteVipSystemDB::ModefyOneData(QString m_tableName, QVariantMap m_map)
{
	if (m_tableName.isEmpty() || m_map.empty()) {
		return false;
	}
	int max_id = 0;
	QString select_max_sql = "select max(md_unique) from e_report_forms";
	QSqlQuery sql = exec(select_max_sql);
	if (lastError().isValid())
	{
		return false;
	}
	else
	{
		while (sql.next())
		{
			max_id = sql.value(0).toInt();
		}
	}
	QString m = genClass(_MD_PRIMARY_KEY, "0");
	QString strSql = genUpdate(m_tableName, m_map, m);

	exec(strSql);
	if (lastError().isValid())
	{
		return false;
	}
	return true;
}
//test code
bool QSqliteVipSystemDB::DeleteOneData(QString m_tableName, QString m_map)
{
	if (m_tableName.isEmpty() || m_map.isEmpty()) {
		return false;
	}
	int max_id = 0;
	QString select_max_sql = "select max("+QString(_MD_PRIMARY_KEY)+") from " + m_tableName;
	QSqlQuery sql = exec(select_max_sql);
	if (lastError().isValid())
	{
		return false;
	}
	else
	{
		while (sql.next())
		{
			max_id = sql.value(0).toInt();
		}
	}
	QString m;// = genClass(_MD_PRIMARY_KEY, "0");
	QString strSql = genDeleteData(m_tableName, m);

	exec(strSql);
	if (lastError().isValid())
	{
		return false;
	}
	return true;
}

QStringList QSqliteVipSystemDB::getModelsByClasses(const QString &classes)
{
	QStringList modelsList;
	QString strClass = genClass(_MD_MODEL, classes);
	QString strSql = genSelect(_MD_TABLE_FORMS_MANE, QStringList(_MD_MODEL), strClass);
	QSqlQuery sql = exec(strSql);

	while (sql.next()) {
		QSqlRecord record = sql.record();
		modelsList.append(record.value(_MD_MODEL).toString());
	}
	return modelsList;
}

QStringList QSqliteVipSystemDB::GetModelsByCamera(const QString& camKey)
{
	QStringList modelsList;
	QSqlQuery sql = exec(QString("SELECT model_id, camera_id, camera_key FROM r_wh_model_camera WHERE camera_key = '%1'").arg(camKey));
	while (sql.next())
	{
		QSqlRecord record = sql.record();
		modelsList.append(record.value("model_id").toString());
	}
	return modelsList;
}

QStringList QSqliteVipSystemDB::getModelsByCamKey(const QString &camKey)
{
	QStringList modelsList;

	QString strClass = genClass(_WF_DB_MD_CAMKEY, camKey);
	QString strSql = genSelect(_WF_DB_MD_TABLE_NAME, QStringList(_WF_DB_MD_MODEL), strClass);
	QSqlQuery sql = exec(strSql);

	while (sql.next()) {
		QSqlRecord record = sql.record();
		modelsList.append(record.value(_WF_DB_MD_MODEL).toString());
	}
	return modelsList;
}

bool QSqliteVipSystemDB::addModel(const QString & strUnique, QVariantMap &vMap)
{
	if (strUnique.isEmpty() || vMap.empty()) {
		return false;
	}

	QString strClass = genClass(_WF_DB_MD_PRIMARY_KEY, strUnique);
	QString strSql = genSelect(_WF_DB_MD_TABLE_NAME, QStringList(), strClass);

	QSqlQuery sql = exec(strSql);
	if (sql.size() > 0) {
		return false;
	}

	strSql = genInsertData(_WF_DB_MD_TABLE_NAME, vMap);

	exec(strSql);
	if (lastError().isValid())
	{
		return false;
	}
	return true;
}

bool QSqliteVipSystemDB::delModelByUnique(const QString &strUnique)
{
	QString str = genDeleteData(_WF_DB_MD_TABLE_NAME, genClass(_WF_DB_MD_PRIMARY_KEY, strUnique));
	QSqlQuery sql = exec(str);
	if (sql.size() == 0) {
		return false;
	}

	return true;
}

bool QSqliteVipSystemDB::getModelInfoByUnique(const QString &strUnique, const QString & strContent, QVariant &result)
{
	QString strClass = genClass(_WF_DB_MD_PRIMARY_KEY, strUnique);
	QString strSql = genSelect(_WF_DB_MD_TABLE_NAME, QStringList(strContent), strClass);
	QSqlQuery sql = exec(strSql);
	if (sql.size() == 0) {
		return false;
	}

	if (lastError().isValid()) {
		return false;
	}
	while (sql.next())
	{
		QSqlRecord records = sql.record();
		result = records.value(strContent);
		return true;
	}

	return false;
}

bool QSqliteVipSystemDB::setModelInfoByUnique(const QString &strUnique, const QString & strContent, QVariant v, int type)
{
	QMutexLocker locker(&m_mxLock);
	QString strSql;
	if (type == 0) {
		QVariantMap vMap;
		vMap.insert(strContent, v);
		QString strClass = genClass(_WF_DB_MD_PRIMARY_KEY, strUnique);
		strSql = genUpdate(_WF_DB_MD_TABLE_NAME, vMap, strClass);
	}

	QSqlQuery sql = exec(strSql);
	if (sql.size() == 0) {
		return false;
	}

	if (lastError().isValid()) {
		return false;
	}

	return true;
}

bool QSqliteVipSystemDB::getRecordByUnique(const QString &strUnique, QSqlRecord &result)
{
	QString strSql = genSelect(_WF_DB_MD_TABLE_NAME, QStringList(), genClass(_WF_DB_MD_PRIMARY_KEY, strUnique));

	QSqlQuery sql = exec(strSql);
	if (sql.size() == 0) {
		return false;
	}

	if (lastError().isValid()) {
		return false;
	}
	while (sql.next())
	{
		QSqlRecord records = sql.record();
		result = records;
		return true;
	}

	return false;
}

bool QSqliteVipSystemDB::addHeader(QString strColumnName, QString strColumnType)
{
	QString strSql = genSelect(_WF_DB_MD_TABLE_NAME);
	QSqlQuery sql = exec(strSql);
	if (sql.size() == 0) {
		return false;
	}

	if (lastError().isValid()) {
		return false;
	}

	while (sql.next()) {
		QSqlRecord records = sql.record();
		int bExist = records.contains(strColumnName);
		if (!bExist) {
			QString strSql = genInsertColumn(_WF_DB_MD_TABLE_NAME, strColumnName, strColumnType);
			QSqlQuery sql = exec(strSql);

			if (sql.size() == 0) {
				return false;
			}

			if (lastError().isValid()) {
				return false;
			}
		}
		return true;
	}

	return true;
}

bool QSqliteVipSystemDB::deleteHeader(QString strColumnName)
{
	QString strSql = genDeleteColumn(_WF_DB_MD_TABLE_NAME, strColumnName);
	QSqlQuery sql = exec(strSql);

	if (sql.size() == 0) {
		return false;
	}

	if (lastError().isValid()) {
		return false;
	}
	return true;
}

bool QSqliteVipSystemDB::increaseJob(QString strModel, QString strKey, QString strValue, QVariantMap &vMap)
{
	QString strTable = _WF_DB_MF_TABLE_PRIFIX + strModel;
	{
		QVariantMap vMapCreate;
		QStringList strList = vMap.keys();
		for (int i = 0; i < strList.size(); i++) {
			vMapCreate.insert(strList[i], "VARCHAR(20)");
		}

		QString strSql = genCreateTabel(strTable, _WF_DB_MF_PRIMARY_KEY, "VARCHAR(90) PRIMARY KEY", vMapCreate);
		QSqlQuery sql = exec(strSql);

		QSqlError err = sql.lastError();
		int t = err.type();
		if (t != QSqlError::NoError)
		{
			return false;
		}
	}

	QString strInsert = genInsertData(strTable, vMap);
	QSqlQuery sql = exec(strInsert);

	if (lastError().isValid()) {
		return false;
	}
	return true;
}

int QSqliteVipSystemDB::genuniqueId(QString m_tableName)
{
	int uId = 0;
	QString select_max_sql = "select * from " + m_tableName;
	QSqlQuery sql = exec(select_max_sql);
	int max = 0;
	while (sql.next())
	{
		uId = sql.value(0).toInt();
		if (max < uId)
			max = uId;
	}
	max += 1;
	return max;
}

bool QSqliteVipSystemDB::checkoutData(QString startDateTime, QString endDateTime, QSqlQuery &sql)
{
	QString strFifter = QString(" where time >'%1' and time < '%2'").arg(startDateTime).arg(endDateTime);
	QString select_sql = QString("select * from ") + _MD_TABLE_FORMS_MANE + strFifter;
	sql = exec(select_sql);
	return true;
}

bool QSqliteVipSystemDB::checkoutData(QString startDateTime, QString endDateTime, QString modelname,QSqlQuery &sql)
{
	QString strFifter = QString(" where time >'%1' and time < '%2' and model='%3'").arg(startDateTime).arg(endDateTime).arg(modelname);
	QString select_sql = QString("select * from ") + _MD_TABLE_FORMS_MANE + strFifter;
	sql = exec(select_sql);
	return true;
}

bool QSqliteVipSystemDB::checkoutData(QString mTime, QSqlQuery &sql)
{
	QString strFifter = QString(" where time ='%1'").arg(mTime);
	QString select_sql = QString("select * from ") + _MD_TABLE_FORMS_MANE + strFifter;
	sql = exec(select_sql);
	return true;
}

bool QSqliteVipSystemDB::checkoutWaringData(QString startDateTime, QString endDateTime, QSqlQuery &sql, int mType)
{
	QString strFifter = QString(" where time >'%1' and time < '%2' and class = '%3' order by time desc ").arg(startDateTime).arg(endDateTime).arg(mType);
	QString select_sql = QString("select * from ") + _MD_WARNINGTABLE +strFifter;
	sql = exec(select_sql);
	return true;
}

QMap<int, QMap<QString, int>> QSqliteVipSystemDB::checkoutData(QString startDateTime, QString endDateTime)
{
	QString strFifter = QString(" where time >'%1' and time < '%2'").arg(startDateTime).arg(endDateTime);
	QString select_sql = QString("select * from ") + _MD_TABLE_FORMS_MANE+strFifter;
	QSqlQuery sql = exec(select_sql);
	QMap<int, QMap<QString, int>> m_channels;
	while (sql.next())
	{
		int channel = sql.value("channel").toInt();
		QString model = sql.value("model").toString();
		if (m_channels.contains(channel))
		{
			if (m_channels.value(channel).contains(model))
			{
				m_channels[channel][model]++;
			}
			else
			{
				int num = 1;
				m_channels[channel].insert(model, num);
			}
		}
		else
		{
			m_channels.insert(channel, QMap<QString, int>());
			if (m_channels.value(channel).contains(model))
			{
				m_channels[channel][model]++;
			}
			else
			{
				int num = 1;
				m_channels[channel].insert(model, num);
			}
		}
	}
	return m_channels;
}

bool QSqliteVipSystemDB::DelDatasByTime(QString minTime)
{
	QString StrTime = QString("time <'%1'").arg(minTime);
	QString select_sql = QString("DELETE FROM %1 WHERE %2").arg(_MD_TABLE_FORMS_MANE).arg(StrTime);
	exec(select_sql);
	return true;
}
bool QSqliteVipSystemDB::DelAllDatas()
{
	QString strSqll = QString("delete from %1 ").arg(_MD_TABLE_FORMS_MANE);
	exec(strSqll);
	strSqll = QString("delete from %1 ").arg("warningtable");
	exec(strSqll);
	return true;
}

bool QSqliteVipSystemDB::DelWarnDataByCount(int model /*=1*/, int nCount/*=100000*/)
{
	//QString m_str1 = "select * from warningtable where (select count(uid) from warningtable)>100 ";
/*	QString strsql = "delete from warningtable where (select count(uid) from warningtable)>100 and uid in(select uid from warningtable order by time desc limit(select count(uid) from warningtable) offset 100)";*/
	QString strLimit1 = QString("select count(uid) from %1 where class = '%3'").arg("warningtable").arg(model);
	QString strLimit2 = QString("uid in(select uid from %1 where class = '%2' order by time desc limit(select count(uid) from %3 where class = '%4') offset %5)").arg("warningtable").arg(model).arg("warningtable").arg(model).arg(nCount);
	QString strSqll = QString("delete from %1 where (%2)>%3 and %4").arg("warningtable").arg(strLimit1).arg(nCount).arg(strLimit2);
	QSqlQuery sql = exec(strSqll);
	//sql.numRowsAffected()
	return true;
}




