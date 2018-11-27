#ifndef _VIRTUALDEVICE_H
#define _VIRTUALDEVICE_H

#include <string>

#include "MV/MVErrorDefine.h"
#include "MV/MVGigEVisionDeviceDefine.h"
#include "MV/MVGiGEVisionGVCPRegisterDefine.h"

#define MV_GEV_BOOTSTRAP_REG_SIZE 0xA000
#define MV_GEV_REG_MEMORY_SIZE    (1 << 20)  // 1MB

#define MV_GEV_XML_FILE_MAX_SIZE  (1 << 20)  // 1MB
#define MV_GEV_XML_URL_LEN        512

// TODO: from XML
#define REG_XML_AcquisitionStart_RegAddr  0x00030804
#define REG_XML_AcquisitionStop_RegAddr   0x00030808
#define REG_XML_Width_RegAddr             0x00030360
#define REG_XML_Height_RegAddr            0x000303a0
#define REG_XML_OffsetX_RegAddr           0x000303e0
#define REG_XML_OffsetY_RegAddr           0x00030420

typedef unsigned char* virtual_addr_t;
class VirtualDevice
{
  public:
	 
    VirtualDevice();
    virtual ~VirtualDevice();

    int Init();
    int DeInit();

    const MV_CC_DEVICE_INFO GetDeviceInfo();

    int GetReg(virtual_addr_t RegAddr, uint32_t& Data);
    inline uint32_t GetReg(virtual_addr_t RegAddr);
    int SetReg(virtual_addr_t RegAddr, const uint32_t Data);
    int GetMem(virtual_addr_t MemAddr, void* Data, const size_t Count);
    int SetMem(virtual_addr_t MemAddr, const void* Data, const size_t Count);

    uint32_t GetAcquisitionState();
    void SetTriggerFrequency(double frequency);
    uint32_t GetControlChannelPrivilege();
    void SetControlChannelPrivilege(uint32_t privilege);
    uint32_t GetPayload();
    uint32_t GetHeartbeatTimeout();

    bool IsCancel();

  private:
    int InitVtMem();

  private:
	  bool                         _bCancel{false};
    MV_CC_DEVICE_INFO			_DeviceInfo;
    std::string                  _strXmlFileName;
    uint32_t                     _nXmlFileSize;

	double                       _fTriggerFrequency{10};  // TODO
    std::string					_strDeviceBinFilename;
    virtual_addr_t				_pVtMem;
	uint32_t					_VtMemSize{1024};

	std::string striniFile;
};


#endif /* _VIRTUALDEVICE_H */
