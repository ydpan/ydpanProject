#ifndef _DEVICEINFO_H
#define _DEVICEINFO_H

#include <string>
#include "DeviceDefine.h"

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

typedef unsigned char uchar;
typedef unsigned int uint32;


class DeviceInfo//设备信息
{
  public:
	DeviceInfo();
    virtual ~DeviceInfo();

    bool Init();

    const LP_DeviceInfo GetDeviceInfo();//获取设备信息
	void SetDeviceInfo(const LP_DeviceInfo &devInfo);

	bool ReadInfobyInIFile(std::string str);//虚拟相机 读取相机IP相关设备配置
	bool ReadXMLbyxmlFile(std::string str);//设备描述文件读取

    bool GetRegisterData(uint32 RegAddr, uint32_t& Data);
    uint32_t GetRegisterData(uint32 RegAddr);
    bool SetRegisterData(uint32 RegAddr, const uint32_t &Data);

    bool GetMemoryData(uint32 MemAddr, void* Data, const size_t Count);
    bool SetMemoryData(uint32 MemAddr, const void* Data, const size_t Count);

    uint32_t GetAcquisitionState();
    void SetTriggerFrequency(double frequency);
    uint32_t GetControlChannelPrivilege();
    void SetControlChannelPrivilege(uint32_t privilege);
    uint32_t GetPayload();
    uint32_t GetHeartbeatTimeout();

  private:
    bool InitDevice();
	void GetLocalIp(unsigned int& nCurrentIp, unsigned int& nCurrentSubNetMask, unsigned int& nDefultGateWay);
	void GetLocalMac(std::string strmac, unsigned int& nMacAddrHigh, unsigned int& nMacAddrLow);
private:
    LP_DeviceInfo				_DeviceInfo;
    std::string                 _strXmlFileName;
    

	double                      _fTriggerFrequency{10};  // TODO
    std::string					_strDeviceBinFilename;
	uint8_t						*m_pMemory;
	char						*pXmlUrl;
	uint32_t					m_totalMemSize{1024};

	std::string striniFile;				//iniFile path

	std::string strLocalIP;				//本设备IP地址
	std::string strSubNetMask;			//掩码
	std::string strDefaultGateWay;		//网关
};


#endif
