#ifndef _DEVICEDEFINE_H_
#define _DEVICEDEFINE_H_

// 设备类型定义
#define LP_UNKNOW_DEVICE        0x00000000          // 未知设备类型，保留意义
#define LP_GIGE_DEVICE          0x00000001          // GigE设备
#define LP_1394_DEVICE          0x00000002          // 1394-a/b 设备
#define LP_USB_DEVICE           0x00000004          // USB3.0 设备
#define LP_CAMERALINK_DEVICE    0x00000008          // CameraLink设备

#define GEV_BOOTSTRAP_REG_SIZE	0xA000
#define GIGE_REG_MEMORY_SIZE    (1 << 20)  // 1MB XML文件起始地址

#define GIGE_XML_FILE_MAX_SIZE  (1 << 20)  // 1MB  XML文件最大内存限制

// TODO: from XML
#define REG_XML_AcquisitionStart_RegAddr  0x00030804
#define REG_XML_AcquisitionStop_RegAddr   0x00030808
#define REG_XML_Width_RegAddr             0x00030360
#define REG_XML_Height_RegAddr            0x000303a0
#define REG_XML_OffsetX_RegAddr           0x000303e0
#define REG_XML_OffsetY_RegAddr           0x00030420

#define ARV_FAKE_CAMERA_MEMORY_SIZE	0x10000

/* To keep in sync with arv-fake-camera.xml */

/* Image format control */
#define ARV_FAKE_CAMERA_REGISTER_SENSOR_WIDTH		0x11c
#define ARV_FAKE_CAMERA_REGISTER_SENSOR_HEIGHT		0x118
#define ARV_FAKE_CAMERA_REGISTER_WIDTH			0x100
#define ARV_FAKE_CAMERA_REGISTER_HEIGHT			0x104
#define ARV_FAKE_CAMERA_REGISTER_X_OFFSET		0x130
#define ARV_FAKE_CAMERA_REGISTER_Y_OFFSET		0x134
#define ARV_FAKE_CAMERA_REGISTER_BINNING_HORIZONTAL	0x108
#define ARV_FAKE_CAMERA_REGISTER_BINNING_VERTICAL	0x10c
#define ARV_FAKE_CAMERA_REGISTER_PIXEL_FORMAT		0x128
#define ARV_FAKE_CAMERA_REGISTER_TEST			0x1f0

#define ARV_FAKE_CAMERA_SENSOR_WIDTH			2048
#define ARV_FAKE_CAMERA_SENSOR_HEIGHT			2048
#define ARV_FAKE_CAMERA_WIDTH_DEFAULT			512
#define ARV_FAKE_CAMERA_HEIGHT_DEFAULT			512
#define ARV_FAKE_CAMERA_BINNING_HORIZONTAL_DEFAULT	1
#define ARV_FAKE_CAMERA_BINNING_VERTICAL_DEFAULT	1
//#define ARV_FAKE_CAMERA_PIXEL_FORMAT_DEFAULT		ARV_PIXEL_FORMAT_MONO_8
#define ARV_FAKE_CAMERA_TEST_REGISTER_DEFAULT		0x12345678

/* Acquisition control */
#define ARV_FAKE_CAMERA_REGISTER_ACQUISITION_MODE	0x12c

#define ARV_FAKE_CAMERA_REGISTER_ACQUISITION_FRAME_PERIOD_US	0x138

#define ARV_FAKE_CAMERA_REGISTER_FRAME_START_OFFSET		0x000
#define ARV_FAKE_CAMERA_REGISTER_ACQUISITION_START_OFFSET	0x020

#define ARV_FAKE_CAMERA_REGISTER_TRIGGER_MODE		0x300
#define ARV_FAKE_CAMERA_REGISTER_TRIGGER_SOURCE		0x304
#define ARV_FAKE_CAMERA_REGISTER_TRIGGER_ACTIVATION	0x308

#define ARV_FAKE_CAMERA_REGISTER_ACQUISITION		0x124
#define ARV_FAKE_CAMERA_REGISTER_EXPOSURE_TIME_US	0x120

#define ARV_FAKE_CAMERA_ACQUISITION_FRAME_RATE_DEFAULT	25.0
#define ARV_FAKE_CAMERA_EXPOSURE_TIME_US_DEFAULT	10000.0

/* Analog control */

#define ARV_FAKE_CAMERA_REGISTER_GAIN_RAW		0x110
#define ARV_FAKE_CAMERA_REGISTER_GAIN_MODE		0x114
/*

*/


/*

*/
typedef unsigned char uchar;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long long uint64;

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
