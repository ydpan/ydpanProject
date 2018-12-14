#ifndef _IDEVICE_H_
#define _IDEVICE_H_
#include <string>
#include "DeviceDefine.h"
#include <boost/asio/ip/udp.hpp>
class IDevice {
public:
	IDevice() {};
	~IDevice() {};
	virtual const LP_DeviceInfo GetDeviceInfo() =0;//获取设备信息
	virtual void SetDeviceInfo(const LP_DeviceInfo &devInfo) =0;

	virtual bool ReadInfobyInIFile(std::string str)=0;//虚拟相机 读取相机IP相关设备配置
	virtual bool ReadXMLbyxmlFile(std::string str) = 0;//设备描述文件读取

	virtual bool GetRegisterData(uint32 RegAddr, uint32& Data) = 0;
	virtual uint32 GetRegisterData(uint32 RegAddr) = 0;
	virtual bool SetRegisterData(uint32 RegAddr, const uint32 &Data) = 0;

	virtual bool GetMemoryData(uint32 MemAddr, void* Data, const size_t Count) = 0;
	virtual bool SetMemoryData(uint32 MemAddr, const void* Data, const size_t Count) = 0;
	
	virtual void UpdateCtroller_time() =0;
	virtual uint32_t GetControlChannelPrivilege() =0;
public:
	double triger_frequency{ 100 };//?
	uint64_t controller_time{ 0 };
	bool bWrite_Access{ false };
	boost::asio::ip::address controler_address;
};




#endif
