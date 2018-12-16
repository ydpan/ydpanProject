#ifndef _DEVICEINFO_H
#define _DEVICEINFO_H
#include "IDevice.h"
#include "pixformat.h"
#include <boost/thread/mutex.hpp>
class DeviceInfo:public IDevice//设备信息
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
public:
    uint32_t GetAcquisitionStatus();//状态
	void	 SetAcquisitionStatus(uint32_t val);

    void SetTriggerFrequency(double frequency); 

    uint32_t GetControlChannelPrivilege();//检查控制权限 control_switchover_key
    void SetControlChannelPrivilege(uint32_t privilege);

	uint32_t GetTrigerModel();// 获取触发模式
	uint32_t GetAcquisitionFramePeridUS();
	uint64_t Get_Sleep_time_for_next_frame(uint64_t next_timestamp);
    uint32_t GetPayload();
    uint32_t GetHeartbeatTimeout();//获取系统超时时间

	uint32_t GetSCP0();
	uint32_t GetStream_Channel_0_Packet_Size();
	uint32_t GetPersistenDefaultGateway();
	uint32_t GetPersistentsubnetMask();
	uint32_t GetPersistenIPAddress();
	uint32_t GetCurrentDefaultGateway();
	uint32_t GetCurrentsubnetMask();
	uint32_t GetCurrentIPAddress();
	uint32_t GetCurrentIPConfig();
	uint32_t GetStreamChannelCapability();
	uint32_t GetSupportIPConfiguration0();
	uint32_t GetDeviceMacAddressHigh0();
	uint32_t GetDeviceMacAddressLow0();
	uint32_t GetGVCPCapability();
	uint32_t GetTimestampTickFrequencyHigh();
	uint32_t GetTimestampTickFrequencyLow();
	uint32_t GetGVCPConfiguration();
	uint32_t GetVersion();
	uint32_t GetLinkSpeed0();
	uint32_t GetNumberOfNetworkInterfaces();
	uint32_t GetUserDefineName();
	uint32_t GetSerialNumber();
	uint32_t GetDeviceVersion();
	uint32_t GetManufactureSpecificInfo();
	uint32_t GetModelName();
	uint32_t GetManufactureName();
	uint32_t GetDeviceMode();
	uint32_t GetNumofStreamChannels();
	uint32_t GetDestinationAddress();
	uint32_t GetDestinationPort();
	//uint32_t GetControlChannelPrivilege();
	uint32_t GetTimestampControl();
	uint32_t GetTimestampLatchedValueHigh();
	uint32_t GetTimestampLatchedValueLow();
	uint32_t GetDeviceLinkSpeed();
	int getcurrentTime();
	void UpdateCtroller_time();
	uint64_t getCtroller_time() { return controller_time; };
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

	boost::mutex m_udpMutex;
};


#endif
