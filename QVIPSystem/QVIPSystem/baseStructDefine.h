#ifndef _BASESTRUCTDEFINE_H_
#define _BASESTRUCTDEFINE_H_

#include <QString>
typedef struct tag_VipMemberInfo{
	int nVipNum{ 0 };//VIP���
	QString strName;//��Ա����
	QString nPhoneNumber;//�ֻ���
	QString strWeixinName;//΢����
	QString strAddress;//��ַ �����
	QString strAbout;//���� 
	double  nMonetary{ 0.0 };//�����ѽ��
	int nDrying_list{ 0 };//ɹ������
	int nIntegration{ 0 };//����
	QVariant reserve1;//����
	QVariant reserve2; // ����
	QVariant reserve3; // ����
}VipMemberInfo;

typedef struct tag_MonetaryRecord{
	int nVipNum{ 0 };
	double nDailyMoney{ 0.0 };
	bool bflags{ false };//���� �����жϼ�¼�Ƿ�ͳ��
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
