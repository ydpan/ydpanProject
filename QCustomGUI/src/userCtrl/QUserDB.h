#ifndef _QUSERSQLITE_H_
#define _QUSERSQLITE_H_

#include <QtCore\qbytearray.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QVariant>
#include <QCryptographicHash>
#include "UserLoginDefine.h"


class QZkDbSqlite : public QSqlDatabase
{
public:
	QZkDbSqlite(const QString& dbName, const QString& dbUser = NULL, const QString& dbPs = NULL)
		: QSqlDatabase("QSQLITE") {
		setDatabaseName(dbName);
		if (!dbUser.isEmpty()) {
			setUserName(dbUser);
			setPassword(dbPs);
		}
	}
	virtual ~QZkDbSqlite() {}
};


class QUserDB : public QZkDbSqlite
{
public:
	QUserDB(const QString& dbPath);
    ~QUserDB();
	inline bool qtByteArrayEqual(const QByteArray& b1, const QByteArray& b2) {
		if (b1.size() != b2.size()) {
			return false;
		}
		return (0 == memcmp(b1.constData(), b2.constData(), b1.size()));
	};
    bool CreateTable();
    bool Replace(const QString& user, const QString& password, int level = 0);
    QStringList Users(int nLevel);
	map_userInfos getAllUserInfos();
	bool Login(const QString& user, const QString& password, int& level);
	void Logout();
    bool Delete(const QString& user);
	int UserLevel(const QString& user);

	int CurLevel() { return m_nCurLevel; }
	QString CurUser() { return m_strCurUser; }
private:
	QString m_strCurUser;
	int m_nCurLevel;
};

#endif

