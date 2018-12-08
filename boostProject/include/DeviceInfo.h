#ifndef _VIRTUALDEVICE_H
#define _VIRTUALDEVICE_H

#include <string>

#include "MV/MVErrorDefine.h"
#include "MV/MVGigEVisionDeviceDefine.h"
#include "MV/MVGiGEVisionGVCPRegisterDefine.h"

#define MV_GEV_BOOTSTRAP_REG_SIZE 0xA000
#define GIGE_REG_MEMORY_SIZE    (1 << 20)  // 1MB

#define GIGE_XML_FILE_MAX_SIZE  (1 << 20)  // 1MB
#define MV_GEV_XML_URL_LEN        512

// TODO: from XML
#define REG_XML_AcquisitionStart_RegAddr  0x00030804
#define REG_XML_AcquisitionStop_RegAddr   0x00030808
#define REG_XML_Width_RegAddr             0x00030360
#define REG_XML_Height_RegAddr            0x000303a0
#define REG_XML_OffsetX_RegAddr           0x000303e0
#define REG_XML_OffsetY_RegAddr           0x00030420

typedef unsigned char uchar;
typedef unsigned int uchar_ptr;
class DeviceInfo
{
  public:
	DeviceInfo();
    virtual ~DeviceInfo();

    int Init();

    const MV_CC_DEVICE_INFO GetDeviceInfo();
    int GetReg(uchar_ptr RegAddr, uint32_t& Data);
    inline uint32_t GetReg(uchar_ptr RegAddr);
    int SetReg(uchar_ptr RegAddr, const uint32_t Data);
    int GetMem(uchar_ptr MemAddr, void* Data, const size_t Count);
    bool SetMem(uchar_ptr MemAddr, const void* Data, const size_t Count);

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
    MV_CC_DEVICE_INFO			_DeviceInfo;
    std::string                  _strXmlFileName;
    uint32_t                     _nXmlFileSize;

	double                       _fTriggerFrequency{10};  // TODO
    std::string					_strDeviceBinFilename;
	uint8_t *					m_pMemory;
	uint32_t					m_totalMemSize{1024};

	std::string striniFile;

	std::string strLocalIP;//本设备IP地址
	std::string strSubNetMask;//掩码
	std::string strDefaultGateWay;//网关
};


#endif /* _VIRTUALDEVICE_H */
