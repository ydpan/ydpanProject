#ifndef _DEVICEDEFINE_H_
#define _DEVICEDEFINE_H_

// 设备类型定义
#define LP_UNKNOW_DEVICE        0x00000000          // 未知设备类型，保留意义
#define LP_GIGE_DEVICE          0x00000001          // GigE设备
#define LP_1394_DEVICE          0x00000002          // 1394-a/b 设备
#define LP_USB_DEVICE           0x00000004          // USB3.0 设备
#define LP_CAMERALINK_DEVICE    0x00000008          // CameraLink设备

typedef struct tag_GigeDeviceInfo 
{
	unsigned int		nIpConfigOption;
	unsigned int		nIpConfigCurrent;                // IP configuration:bit31-static bit30-dhcp bit29-lla
	unsigned int		nCurrentIp;                      // curtent ip
	unsigned int        nCurrentSubNetMask;              // curtent subnet mask
	unsigned int        nDefultGateWay;                  // current gateway
	unsigned char       cManufacturerName[32];
	unsigned char       cModelName[32];
	unsigned char       cDeviceVersion[32];
	unsigned char       cManufacturerSpecificInfo[48];
	unsigned char       cSerialNumber[16];
	unsigned char       cUserDefinedName[16];
	unsigned int        nNetExport;                      // 网口IP地址
}LP_GigeDeviceInfo;

typedef struct tag_DeviceInfo
{
	unsigned short			nMaxorVer{ 0 };
	unsigned short			nMinorVer{ 0 };
	unsigned int			nDeviceMode{ 0 };
	unsigned int			nMacAddrHigh{ 0 };               // MAC 地址
	unsigned int			nMacAddrLow{ 0 };
	unsigned int			nTLayerType{ 0 };  //设备传输协议类型  
	LP_GigeDeviceInfo		st_gigeInfo{0};
}LP_DeviceInfo;


#endif // _DEVICEDEFINE_H_
