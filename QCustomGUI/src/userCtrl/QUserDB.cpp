#include "QUserDB.h"
#include <QtCore\quuid.h>
#include <QtCore\qcryptographichash.h>

#pragma execution_character_set("utf-8")

#define _TP_SQLITE_CRETAE_USERS_TABLE \
"CREATE TABLE IF NOT EXISTS user(id VARCHAR(255) \
, salt BLOB \
, password BLOB \
, level INT(32) \
, stamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP \
, PRIMARY KEY(id))"

#define _TP_QSQLITE_REPLACE_USER "REPLACE INTO user(id, salt, password, level) VALUES('%1', :data_salt, :data_password, '%2')"
#define _TP_QSQLITE_SELECT_ALL "SELECT id, level FROM user"
#define _TP_QSQLITE_SELECT_ALLINFO "SELECT id, level, password FROM user"
#define _TP_QSQLITE_SELECT_USERS_BY_LEVEL "SELECT id, level FROM user WHERE level >= '%1'"
#define _TP_QSQLITE_SELECT_USER "SELECT id, level, salt, password FROM user WHERE id = '%1'"
#define _TP_QSQLITE_DELETE_USER "DELETE FROM user WHERE id='%1'"

#define _DB_USER    "user"
#define _DB_PS      "hzleaper_administrator"

QUserDB::QUserDB(const QString& dbPath)
    : QZkDbSqlite(dbPath + "users.db", _DB_USER, _DB_PS)
	, m_strCurUser(NO_USER_NAME)
	, m_nCurLevel(USER_LEVEL_NOUSER)
{

}

QUserDB::~QUserDB()
{

}

bool QUserDB::CreateTable()
{
    if (!isOpen() && !open())
    {
        return false;
    }
    QSqlQuery sql = exec(_TP_SQLITE_CRETAE_USERS_TABLE);
    if (QSqlError::NoError != lastError().type())
    {
        return false;
    }
    //Replace("root", "hzleaper");
    return true;
}

bool QUserDB::Replace(const QString& user, const QString& password, int level /*= 0*/)
{
    QByteArray ps = password.toUtf8();
    QByteArray salt;
    for (int i = 0; i < 10; ++i)
    {
        salt.append(QUuid::createUuid().toRfc4122());
    }
    ps.append(salt);
    ps = QCryptographicHash::hash(ps, QCryptographicHash::Sha3_512);

    QString strCmd(_TP_QSQLITE_REPLACE_USER);
    strCmd = strCmd.arg(user, QString::number(level));
    QSqlQuery sql(*this);
    sql.prepare(strCmd);
    sql.bindValue(":data_salt", salt, QSql::In);
    sql.bindValue(":data_password", ps, QSql::In);
    return sql.exec();
}

QStringList QUserDB::Users(int nLevel)
{
    QStringList idList;
    QSqlQuery sql;
    if (nLevel < 0)
    {
        sql = exec(_TP_QSQLITE_SELECT_ALL);
    }
    else
    {
        sql = exec(QString(_TP_QSQLITE_SELECT_USERS_BY_LEVEL).arg(QString::number(nLevel)));
    }
    while (sql.next())
    {
        idList.append(sql.value("id").toString());
    }
    return idList;
}

map_userInfos QUserDB::getAllUserInfos()
{
	QSqlQuery sql;
	map_userInfos rltInfos;
	sql = exec(_TP_QSQLITE_SELECT_ALLINFO);
	int nIndex = 0;
	while (sql.next())
	{
		ST_UserInfo itemInfo;
		itemInfo.strName = sql.value("id").toString();
		itemInfo.nlevel = sql.value("level").toInt();
		itemInfo.strpasswd = sql.value("password").toString();
		rltInfos[nIndex] = itemInfo;
		nIndex++;
	}
	return rltInfos;
}

bool QUserDB::Login(const QString& user, const QString& password, int& level)
{
    QString strCmd(_TP_QSQLITE_SELECT_USER);
    strCmd = strCmd.arg(user);
    QSqlQuery sql = exec(strCmd);
    if (QSqlError::NoError != lastError().type() || !sql.next())
    {
        return false;
    }
    QByteArray salt = sql.value("salt").toByteArray();
    QByteArray ps = sql.value("password").toByteArray();
    //
    QByteArray origin = password.toUtf8();
    origin.append(salt);
    origin = QCryptographicHash::hash(origin, QCryptographicHash::Sha3_512);
    if (!qtByteArrayEqual(origin, ps))
    {
        return false;
    }
    level = sql.value("level").toInt();
    
	m_strCurUser = user;
	m_nCurLevel = level;
    return true;
}

void QUserDB::Logout()
{
	m_strCurUser = NO_USER_NAME;
	m_nCurLevel = USER_LEVEL_NOUSER;
}


bool QUserDB::Delete(const QString& user)
{
    QString strCmd(_TP_QSQLITE_DELETE_USER);
    strCmd = strCmd.arg(user);
    QSqlQuery sql = exec(strCmd);
    if (QSqlError::NoError != lastError().type())
    {
        return false;
    }

    return true;
}

int QUserDB::UserLevel(const QString& user)
{
	QString strCmd = "SELECT level FROM user WHERE id = '%1'";
	strCmd = strCmd.arg(user);
	QSqlQuery sql = exec(strCmd);
	if (QSqlError::NoError != lastError().type()) {
		return false;
	}
	while (sql.next()) {
		return sql.value("level").toInt();
	}
	return -1;
}
