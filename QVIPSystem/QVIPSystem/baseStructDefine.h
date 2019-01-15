#ifndef _BASESTRUCTDEFINE_H_
#define _BASESTRUCTDEFINE_H_

#include <QString>
typedef struct tag_VipMemberInfo{
	int nUid{0};
	int nVipNum{ 0 };//VIP编号
	QString strName;//成员名字
	QString nPhoneNumber;//手机号
	QString strWeixinName;//微信名
	QString strAddress;//地址 房间号
	QString strAbout;//关于 
	double  nMonetary{ 0.0 };//总消费金额
	int nDrying_list{ 0 };//晒单次数
	int nIntegration{ 0 };//积分
	QVariant reserve1;//备用
	QVariant reserve2; // 备用
	QVariant reserve3; // 备用
	QString strTime;
	QString strUpdateTime;
}VipMemberInfo;

typedef struct tag_MonetaryRecord{
	int nVipNum{ 0 };
	QString strName;
	double nDailyMoney{ 0.0 };
	int nDryListCount{ 0 };
	bool bflags{ false };//备用 用作判断记录是否备统计
	QString strTime;
	QString strAbout;
}MonetaryRecord;

typedef struct tag_DryListRecorde {
	int nVipNum{ 0 };
	QString strName;
	
	bool bflags{ false };
	QString strTime;
}DryListRecorde;


#define _DBFILE_NAME_						"vipDB.db"
#define _TABLE_USERINFO_TABLE_				"userInfo"
#define _TABLE_TOTAL_TABLE_					"TotalMonetaryTable"
#define _TABLE_MOMEY_TABLE_					"ConmuseTable"
#define _TABLE_DRY_TABLE_					"Drying_listTable"
#define _TABLE_SUBINTEGRATION_TABLE_		"SubIntegrationTable"

#define _PRIMARY_KEY_						"uid"
#define _VIPIP_								"vipiD"
#define _STRNAME_							"strName"
#define _PHONENUM_							"phoneNum"
#define _ADDRESS_							"address"
#define _CHARTNAME_							"chartName"
#define _ABOUT_								"about"
#define _RESERVE1_							"reserve1"
#define _RESERVE2_							"reserve2"
#define _RESERVE3_							"reserve3"
#define _TIME_								"time"
#define _UPDATETIME_						"updatetime"
#define _INTEGRATION_ "Integration"
#define _MONETARY_  "monetary"
#define _DRYLIST_  "Drying_list"
#endif // 
