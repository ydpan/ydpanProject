#ifndef QSQLITEWHEELHUBWF_H
#define QSQLITEWHEELHUBWF_H

#include <QtSql\qsqldatabase.h>
#include <QtCore\qobject.h>
#include "QSqliteBase.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTime>
#include "qmutex"
#include "baseStructDefine.h"

#define _WF_DB_MD_TABLE_NAME   "e_wh_models"
#define _WF_DB_MD_PRIMARY_KEY  "md_unique"
#define _WF_DB_MD_CAMKEY       "md_camKey"
#define _WF_DB_MD_INDEX        "md_index"
#define _WF_DB_MD_CALI         "md_cali"
#define _WF_DB_MD_COUNT        "md_count"
#define _WF_DB_MD_MODEL        "md_model"
#define _WF_DB_MD_ID           "md_id"
#define _WF_DB_MD_TEXT         "md_text"
#define _WF_DB_MD_STAMP        "md_stamp"

#define _WF_DB_MF_TABLE_PRIFIX  "e_wf_model_info"
#define _WF_DB_MF_PRIMARY_KEY   "mf_time"
#define _WF_DB_MF_MODEL         "mf_model"
#define _WF_DB_MF_UNIQUE        "mf_unique"
#define _WF_DB_MF_ERROR_TYPE    "mf_error"
#define _WF_DB_MF_ANGLE         "md_angle"

#define _WF_UNIQUE_SPLIT                      "_-_"
#define _WF_PRIMARY_SPLIT                     "__"

/*myself     */
#define	  _MD_PRIMARY_KEY				"uid"	
#define   _MD_TABLE_FORMS_MANE			"wftable"	//数据表名称

#define   _MD_MODEL						"model"		//model 模型名称
#define   _MD_DIAMETER					"diameter"	//直径
#define   _MD_HIGHT						"hight"			//高度
#define   _MD_CORRELATE					"correlate"		//相似度
#define   _MD_CHANNEL					"channel"		//通道
#define   _MD_PICPATH					"picpath"      //图片路径
#define   _MD_ROTATE					"rotate"//辐条个数
#define   _MD_UPDATE_TIME				"time"	//最后更新的时间
#define   _MD_PIC						"pic"   //图片


#define  _MD_TIMETABLE					"wftimetable"
#define  _MD_TYPE						"type"
#define  _MD_STARTTIME					"starttime"
#define  _MD_ENDTIME					"endtime"

#define _MD_WARNINGTABLE				"warningtable"
#define _MD_MESSAGE						"message"
#define _MD_CLASS                       "class"


class QSqliteVipSystemDB : public QSqliteBase
{
public:
	QSqliteVipSystemDB(const QString& dbPath);
	~QSqliteVipSystemDB();

	virtual bool    InitDatabase();
	virtual void    FreeDatabase();

	bool AddNewVipMemberInfo(VipMemberInfo &info);
	int getVipMemberMaxID();
	virtual bool CreateTables(QString m_tableName, QString m_primary, QVariantMap m_map);
	virtual bool InsertOneData(QString m_tableName, QVariantMap m_map);
	virtual bool ModefyOneData(QString m_tableName, QVariantMap m_map);
	virtual bool DeleteOneData(QString m_tableName, QString m_map);
	virtual QStringList GetModelsByCamera(const QString& camKey);
	bool addModel(const QString & strUnique, QVariantMap &vMap);
	virtual QStringList getModelsByCamKey(const QString &cam);
	virtual bool delModelByUnique(const QString &strUnique);
	virtual bool getModelInfoByUnique(const QString &strUnique, const QString & strContent, QVariant &result);
	virtual bool getRecordByUnique(const QString &strUnique, QSqlRecord &result);
	virtual bool setModelInfoByUnique(const QString &strUnique, const QString & strContent, QVariant result, int type = 0);
	virtual bool addHeader(QString strColumnName, QString strColumnType);
	virtual bool deleteHeader(QString strColumnName);
	virtual bool increaseJob(QString strModel, QString strKey, QString strValue, QVariantMap &vMap);
	
	virtual QStringList getModelsByClasses(const QString &classes);//通过班次获取轮毂型号
	virtual int genuniqueId(QString m_tableName);
	virtual bool addData(QVariantMap m_map);
	virtual QMap<int, QMap<QString, int>> checkoutData(QString startDateTime, QString endDateTime);
	virtual bool checkoutData(QString startDateTime, QString endDateTime, QSqlQuery &sql);
	virtual bool checkoutData(QString startDateTime, QString endDateTime, QString modelname, QSqlQuery &sql);
	virtual bool checkoutData(QString mTime, QSqlQuery &sql);
	virtual bool DelDatasByTime(QString minTime);
	virtual bool ReadOutTimeData(QMap<QString, QMap<QString, QTime>> &m_MapString);
	virtual bool UpdateTimeData(QMap<QString, QMap<QString, QTime>> m);
	virtual bool checkoutWaringData(QString startDateTime, QString endDateTime, QSqlQuery &sql, int mType);//查询报警信息记录
	//virtual bool AddOneWarningMessage(WarnMessage m_messageInfo);
	virtual bool AddOneData(QVariantMap m_map);
	virtual bool DelWarnDataByCount(int model = 1, int nCount = 100000);
	virtual bool DelAllDatas();
private:
	QMutex m_mxLock;
	int uniqueId;
};

#endif // QSQLITEWHEELHUBWF_H
