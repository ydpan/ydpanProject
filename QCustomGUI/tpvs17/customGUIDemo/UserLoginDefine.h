#ifndef _USER_GLOBAL_H_
#define _USER_GLOBAL_H_
#include <QString>
typedef struct tag_CustomUserInfo {
	QString strName;
	QString passwd;
	QString strAbout;
	int mLevel{0};

}UserInfo;

typedef enum LogIndState {
	EM_LOGIN = 0,
	EM_LOGOUT = 1
}em_LoginState;

enum USER_LEVEL
{
	USER_LEVEL_LOWEST = 0,
	USER_LEVEL_NOUSER = 1,
	USER_LEVEL_OPT = 4,
	USER_LEVEL_ADM = 5,
	USER_LEVEL_FULL = 9
};

#define NO_USER_NAME "nouser"

typedef QMap<int, UserInfo> map_userInfos;

static const QMap<int, QString> sc_level = {
	{ USER_LEVEL_ADM, QString::fromLocal8Bit("管理员") },
	{ USER_LEVEL_OPT, QString::fromLocal8Bit("操作员") },
	{ USER_LEVEL_FULL, QString::fromLocal8Bit("根管理员") }
};
#endif // 
