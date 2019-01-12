#ifndef _QSQLITEBASE_H_
#define _QSQLITEBASE_H_

#include <QtSql\qsqldatabase.h>
#include <QtSql\qsqlquery.h>
#include <QtSql\qsqlerror.h>
#include <QtSql\qsqlrecord.h>
#include <QtSql\qsqlfield.h>
#include <QtSql\qsqldriver.h>
#include <QtCore\qstring.h>
#include <QtCore\qstringlist.h>
#include <QtCore\qvariant.h>
#include <QtCore\qbuffer.h>
#include <QVariantMap>
class QSqliteBase : public QSqlDatabase
{
public:
	QSqliteBase(const QString& dbName, const QString& dbUser = NULL, const QString& dbPs = NULL);
	virtual ~QSqliteBase();
	QString genInsertData(const QString & strTableName, const QVariantMap &vMap);
	QString genInsertColumn(const QString & strTableName, QString strName, QString strType);
	QString genClass(QString strName, QString strValue);
	QString genSelect(const QString & strTableName, QStringList strSelect = QStringList(), QString strClass = QString());
	QString genUpdate(const QString & strTableName, QVariantMap &vMap, QString strClass=QString());
	QString genDeleteData(const QString & strTableName, QString strClass = QString());
	QString genDeleteColumn(const QString & strTableName, QString strColumnName);
	QString genCreateTabel(const QString strTable, QString strPrimary, QString strType, QVariantMap &vMap);
};

#endif // QSQLITEGENERAL_H
