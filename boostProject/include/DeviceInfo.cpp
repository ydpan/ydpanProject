#include <stdlib.h>
#include <fstream>
#define WINDOWS
#include <stdlib.h>
#include <stdio.h>

#ifdef WINDOWS
#include <winsock2.h>
#include <Iphlpapi.h>
// #pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <iostream>
#include "DeviceInfo.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;

DeviceInfo::DeviceInfo():m_pMemory(NULL), m_totalMemSize(GIGE_REG_MEMORY_SIZE + GIGE_XML_FILE_MAX_SIZE)
{
	m_pMemory = new uchar[m_totalMemSize];
	memset(m_pMemory, 0, m_totalMemSize);

	_strXmlFileName = "C:\\1Documents\\LeaperProject\\ydpanProject\\virtual-camera.zip";
	striniFile = "C:\\1Documents\\LeaperProject\\ydpanProject\\VirtualDevice.ini";
}

DeviceInfo::~DeviceInfo()
{
	if (m_pMemory)
	{
		delete[] m_pMemory;
		m_pMemory = NULL;
	}
}

int DeviceInfo::Init()
{
    int nRet = MV_OK;
    if ((nRet = InitDevice()) != MV_OK)
    {
        return nRet;
    }

    return nRet;
}

bool DeviceInfo::InitDevice()
{
	bool ret = true;

    unsigned int nDefMacAddrHigh, nDefMacAddrLow, nDefCurrentIp, nDefCurrentSubNetMask, nDefDefultGateWay;
	std::string macaddr;
	GetLocalMac(macaddr, nDefMacAddrHigh, nDefMacAddrLow);
    GetLocalIp(nDefCurrentIp, nDefCurrentSubNetMask, nDefDefultGateWay);

	boost::property_tree::ptree m_pt, tag_setting;
	read_ini(striniFile, m_pt);
	
    int Tmp;
	tag_setting = m_pt.get_child("INFO_CC");
	_DeviceInfo.nMajorVer = tag_setting.get<unsigned short>("MajorVer");
	_DeviceInfo.nMinorVer = tag_setting.get<unsigned short>("MinorVer");
	_DeviceInfo.nDeviceMode = tag_setting.get<unsigned int>("DeviceMode");

    _DeviceInfo.nMacAddrHigh       = nDefMacAddrHigh;
    _DeviceInfo.nMacAddrLow        = nDefMacAddrLow;
    _DeviceInfo.nTLayerType        = MV_GIGE_DEVICE;

	boost::property_tree::ptree info_get_setting;
	info_get_setting = m_pt.get_child("INFO_GEV");
	_DeviceInfo.stGigEInfo.nIpCfgOption = info_get_setting.get<unsigned int>("IpCfgOption");
	_DeviceInfo.stGigEInfo.nIpCfgCurrent = info_get_setting.get<unsigned int>("IpCfgCurrent");
    _DeviceInfo.stGigEInfo.nCurrentIp         = nDefCurrentIp;
    _DeviceInfo.stGigEInfo.nCurrentSubNetMask = nDefCurrentSubNetMask;
    _DeviceInfo.stGigEInfo.nDefultGateWay     = nDefDefultGateWay;

    memcpy(_DeviceInfo.stGigEInfo.chManufacturerName, info_get_setting.get<std::string>("ManufacturerName").c_str(), 32);
    memcpy(_DeviceInfo.stGigEInfo.chModelName, info_get_setting.get<std::string>("ModelName").c_str(), 32);
    memcpy(_DeviceInfo.stGigEInfo.chDeviceVersion, info_get_setting.get<std::string>("DeviceVersion").c_str(), 32);
    memcpy(_DeviceInfo.stGigEInfo.chManufacturerSpecificInfo, info_get_setting.get<std::string>("ManufacturerSpecificInfo").c_str(), 48);
    memcpy(_DeviceInfo.stGigEInfo.chSerialNumber, info_get_setting.get<std::string>("SerialNumber").c_str(), 16);
    memcpy(_DeviceInfo.stGigEInfo.chUserDefinedName, info_get_setting.get<std::string>("UserDefinedName").c_str(), 16);

    // Device info
    Tmp = (_DeviceInfo.nMinorVer << 16) + _DeviceInfo.nMajorVer;
    SetMem((uchar_ptr)MV_REG_Version, (&Tmp), 4);
    SetMem((uchar_ptr)MV_REG_DeviceMode, (&_DeviceInfo.nDeviceMode), 4);
    SetMem((uchar_ptr)MV_REG_DeviceMACAddressHigh0, (&_DeviceInfo.nMacAddrHigh), 4);
    SetMem((uchar_ptr)MV_REG_DeviceMACAddressLow0, (&_DeviceInfo.nMacAddrLow), 4);
    SetMem((uchar_ptr)MV_REG_NetworkInterfaceCapability0, (&_DeviceInfo.stGigEInfo.nIpCfgOption), 4);
    SetMem((uchar_ptr)MV_REG_NetworkInterfaceConfiguration0, (&_DeviceInfo.stGigEInfo.nIpCfgCurrent), 4);
    SetMem((uchar_ptr)MV_REG_CurrentIPAddress0, (&_DeviceInfo.stGigEInfo.nCurrentIp), 4);
    SetMem((uchar_ptr)MV_REG_CurrentSubnetMask0, (&_DeviceInfo.stGigEInfo.nCurrentSubNetMask), 4);
    SetMem((uchar_ptr)MV_REG_CurrentDefaultGateway0, (&_DeviceInfo.stGigEInfo.nDefultGateWay), 4);
    SetMem((uchar_ptr)MV_REG_ManufacturerName, (&_DeviceInfo.stGigEInfo.chManufacturerName), 32);
    SetMem((uchar_ptr)MV_REG_ModelName, (&_DeviceInfo.stGigEInfo.chModelName), 32);
    SetMem((uchar_ptr)MV_REG_DeviceVersion, (&_DeviceInfo.stGigEInfo.chDeviceVersion), 32);
    SetMem((uchar_ptr)MV_REG_ManufacturerInfo, (&_DeviceInfo.stGigEInfo.chManufacturerSpecificInfo), 48);
    SetMem((uchar_ptr)MV_REG_SerialNumber, (&_DeviceInfo.stGigEInfo.chSerialNumber), 16);
    SetMem((uchar_ptr)MV_REG_UserdefinedName, (&_DeviceInfo.stGigEInfo.chUserDefinedName), 16);

    // MISC
    SetReg((uchar_ptr)MV_REG_ControlChannelPrivilege, 1);
    SetReg((uchar_ptr)MV_REG_HeartbeatTimeout, 1000);
    SetReg((uchar_ptr)MV_REG_TimestampTickFrequencyHigh, 0);
    SetReg((uchar_ptr)MV_REG_TimestampTickFrequencyLow, 1000000000);
    SetReg((uchar_ptr)MV_REG_StreamChannelPacketSize0, 2000);
    SetReg((uchar_ptr)MV_REG_NumberofStreamChannels, 1);

    // XML defined register
    SetReg((uchar_ptr)REG_XML_AcquisitionStart_RegAddr, 0x0000);
    SetReg((uchar_ptr)REG_XML_AcquisitionStop_RegAddr, 0x0001);
    SetReg((uchar_ptr)REG_XML_Width_RegAddr, 0x0c00);
    SetReg((uchar_ptr)REG_XML_Height_RegAddr, 0x0800);
    SetReg((uchar_ptr)REG_XML_OffsetX_RegAddr, 0x0000);
    SetReg((uchar_ptr)REG_XML_OffsetY_RegAddr, 0x0000);

    // XML file
    char szXmlUrl[MV_GEV_XML_URL_LEN];
    fstream XmlFile;
    XmlFile.open(_strXmlFileName.c_str(), ios::in | ios::binary);
    XmlFile.seekg(0, ios::end);
    _nXmlFileSize = XmlFile.tellg();
    XmlFile.seekg(0, ios::beg);
    if (_nXmlFileSize <= GIGE_XML_FILE_MAX_SIZE)
    {
        sprintf(szXmlUrl, "Local:%s;%x;%x", _strXmlFileName.c_str(), GIGE_REG_MEMORY_SIZE, _nXmlFileSize);
        SetMem(uchar_ptr(MV_REG_FirstURL), szXmlUrl, strlen(szXmlUrl));
        XmlFile.read((char*)m_pMemory + GIGE_REG_MEMORY_SIZE, _nXmlFileSize);
    }
    else
    {
        cout << "Read XML file (" << _strXmlFileName << ") fail!!" << endl;
        return false;
    }

    return ret;
}


const MV_CC_DEVICE_INFO DeviceInfo::GetDeviceInfo()
{
    return _DeviceInfo;
}

int DeviceInfo::GetReg(uchar_ptr RegAddr, uint32_t& Data)
{
    return GetMem(RegAddr, &Data, 4);
}

inline uint32_t DeviceInfo::GetReg(uchar_ptr RegAddr)
{
    uint32_t Data = -1;

    GetMem(RegAddr, &Data, 4);
    return Data;
}

int DeviceInfo::SetReg(uchar_ptr RegAddr, const uint32_t Data)
{
    return SetMem(RegAddr, &Data, 4);
}

int DeviceInfo::GetMem(uchar_ptr MemAddr, void* Data, const size_t Count)
{
    int nRet = MV_OK;

    if (MemAddr <= m_totalMemSize)
    {
        uint8_t *memdata = m_pMemory + MemAddr;
        memcpy((unsigned char*)Data, (unsigned char*)memdata, Count);
    }
    else
    {
        nRet = MV_E_PARAMETER;
    }

    return nRet;
}

bool DeviceInfo::SetMem(uchar_ptr MemAddr, const void* Data, const size_t Count)
{
    int nRet = MV_OK;

    if (MemAddr <= m_totalMemSize)
    {
        uint8_t *pMemAddr = m_pMemory + MemAddr;
        memcpy((unsigned char*)pMemAddr, (unsigned char*)Data, Count);
    }
    else
    {
        nRet = MV_E_PARAMETER;
    }

    return false;
}

uint32_t DeviceInfo::GetAcquisitionState()
{
    return GetReg((uchar_ptr)REG_XML_AcquisitionStart_RegAddr);
}

void DeviceInfo::SetTriggerFrequency(double frequency)
{
    _fTriggerFrequency = frequency;
}

uint32_t DeviceInfo::GetControlChannelPrivilege()
{
    return GetReg((uchar_ptr)MV_REG_ControlChannelPrivilege);
}

void DeviceInfo::SetControlChannelPrivilege(uint32_t privilege)
{
    SetReg((uchar_ptr)MV_REG_ControlChannelPrivilege, privilege);
}

uint32_t DeviceInfo::GetPayload()
{
    int width = GetReg((uchar_ptr)REG_XML_Width_RegAddr);
    int height = GetReg((uchar_ptr)REG_XML_Height_RegAddr);

    return width * height;
}

uint32_t DeviceInfo::GetHeartbeatTimeout()
{
    return GetReg((uchar_ptr)MV_REG_HeartbeatTimeout);
}

void DeviceInfo::GetLocalIp(unsigned int& nCurrentIp, unsigned int& nCurrentSubNetMask, unsigned int& nDefultGateWay)
{
	strLocalIP = string("192.168.8.153");
	strSubNetMask = string("255.255.255.0");
	strDefaultGateWay = string("192.168.8.1");
	nCurrentIp = inet_addr(strLocalIP.c_str());
	nCurrentSubNetMask = inet_addr(strSubNetMask.c_str());
	nDefultGateWay = inet_addr(strDefaultGateWay.c_str());
	nCurrentIp = ntohl(nCurrentIp);
	nCurrentSubNetMask = ntohl(nCurrentSubNetMask);
	nDefultGateWay = ntohl(nDefultGateWay);
}

void DeviceInfo::GetLocalMac(std::string strmac, unsigned int& nMacAddrHigh, unsigned int& nMacAddrLow)
{
	long long nMacAddr;
	memcpy((unsigned char*)&nMacAddr, (unsigned char*)(strmac.c_str()), sizeof(strmac.size()));
	nMacAddrHigh = (nMacAddr & 0x0000000000ffff);
	nMacAddrHigh = ntohl(nMacAddrHigh);
	nMacAddrLow = (nMacAddr & 0xffffffffff0000) >> 16;
	nMacAddrLow = ntohl(nMacAddrLow);
}

