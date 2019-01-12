#ifndef _BASESTRUCTDEFINE_H_
#define _BASESTRUCTDEFINE_H_

#include <QString>
typedef struct tag_VipMemberInfo{
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
}VipMemberInfo;

typedef struct tag_MonetaryRecord{
	int nVipNum{ 0 };
	double nDailyMoney{ 0.0 };
	bool bflags{ false };//备用 用作判断记录是否备统计
}MonetaryRecord;

typedef struct tag_DryListRecorde {
	int nVipNum{ 0 };
	int nDryListCount{ 0 };
	bool bflags{ false };
}DryListRecorde;


#define _DBFILE_NAME_						"vipDB.db"
#define _TABLE_USERINFO_TABLE_				"userInfo"
#define _TABLE_TOTAL_TABLE_					"TotalMonetaryTable"
#define _TABLE_MOMEY_TABLE_					"MonetaryTable"
#define _TABLE_DRY_TABLE_					"Drying_listTable"
#define _TABLE_SUBINTEGRATION_TABLE_		"SubIntegrationTable"

#define _PRIMARY_KEY_						"uid"



#endif // 
