#include <stdlib.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "DeviceInfo.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "gvcp.h"
#define WINDOWS
#ifdef WINDOWS
#include <winsock2.h>
#include <Iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif


using namespace std;

DeviceInfo::DeviceInfo():m_pMemory(NULL), m_totalMemSize(GIGE_REG_MEMORY_SIZE + GIGE_XML_FILE_MAX_SIZE)
{
	m_pMemory = new unsigned char[m_totalMemSize];
	memset(m_pMemory, 0, m_totalMemSize);

	pXmlUrl = new char[ARV_GVBS_XML_URL_SIZE];

	_strXmlFileName = "E:\\LeaperProject\\ydpanProject\\virtual-camera.zip";
	striniFile = "E:\\LeaperProject\\ydpanProject\\VirtualDevice.ini";
}

DeviceInfo::~DeviceInfo()
{
	if (m_pMemory)
	{
		delete[] m_pMemory;
		m_pMemory = NULL;
	}
	if (pXmlUrl) {
		delete[] pXmlUrl;
		pXmlUrl = NULL;
	}
}

bool DeviceInfo::Init()
{
    return InitDevice();
}

bool DeviceInfo::InitDevice()
{
	bool ret = true;

    unsigned int nDefMacAddrHigh, nDefMacAddrLow, nDefCurrentIp, nDefCurrentSubNetMask, nDefDefultGateWay;
	std::string macaddr;
	GetLocalMac(macaddr, nDefMacAddrHigh, nDefMacAddrLow);
    GetLocalIp(nDefCurrentIp, nDefCurrentSubNetMask, nDefDefultGateWay);

	_DeviceInfo.nMacAddrHigh = nDefMacAddrHigh;
	_DeviceInfo.nMacAddrLow = nDefMacAddrLow;
	_DeviceInfo.nTLayerType = LP_GIGE_DEVICE;//设备类型
	_DeviceInfo.st_gigeInfo.nCurrentIp = nDefCurrentIp;
	_DeviceInfo.st_gigeInfo.nCurrentSubNetMask = nDefCurrentSubNetMask;
	_DeviceInfo.st_gigeInfo.nDefultGateWay = nDefDefultGateWay;

	ReadInfobyInIFile(striniFile);
	SetDeviceInfo(_DeviceInfo);

    // MISC
    SetRegisterData((uint32)ARV_GVBS_CONTROL_CHANNEL_PRIVILEGE_OFFSET, 1);
    SetRegisterData((uint32)ARV_GVBS_HEARTBEAT_TIMEOUT_OFFSET, 1000);
    SetRegisterData((uint32)ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_HIGH_OFFSET, 0);
    SetRegisterData((uint32)ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_LOW_OFFSET, 1000000000);
    SetRegisterData((uint32)ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_OFFSET, 2000);
    SetRegisterData((uint32)ARV_GVBS_N_STREAM_CHANNELS_OFFSET, 1);
	ReadXMLbyxmlFile(_strXmlFileName);

    return ret;
}

bool DeviceInfo::GetRegisterData(uint32 RegAddr, uint32_t &Data)
{
    return GetMemoryData(RegAddr, &Data, 4);
}

uint32_t DeviceInfo::GetRegisterData(uint32 RegAddr)
{
    uint32_t Data = -1;

    GetMemoryData(RegAddr, &Data, 4);
    return Data;
}

bool DeviceInfo::SetRegisterData(uint32 RegAddr, const uint32_t &Data)
{
    return SetMemoryData(RegAddr, &Data, 4);
}

bool DeviceInfo::GetMemoryData(uint32 MemAddr, void* Data, const size_t Count)
{
    if (MemAddr <= m_totalMemSize)
    {
        uint8_t *pMemdata = m_pMemory + MemAddr;
        memcpy((unsigned char*)Data, (unsigned char*)pMemdata, Count);
    }
    else
    {
		return false;
    }

    return true;
}

bool DeviceInfo::SetMemoryData(uint32 MemAddr, const void* Data, const size_t Count)
{
    if (MemAddr <= m_totalMemSize)
    {
        uint8_t *pMemAddr = m_pMemory + MemAddr;
        memcpy((unsigned char*)pMemAddr, (unsigned char*)Data, Count);
    }
    else
    {
		return false;
    }

    return true;
}

uint32_t DeviceInfo::GetAcquisitionState()
{
    return GetRegisterData((uint32)REG_XML_AcquisitionStart_RegAddr);
}

void DeviceInfo::SetTriggerFrequency(double frequency)
{
    _fTriggerFrequency = frequency;
}

uint32_t DeviceInfo::GetControlChannelPrivilege()
{
    return GetRegisterData((uint32)ARV_GVBS_CONTROL_CHANNEL_PRIVILEGE_OFFSET);
}

void DeviceInfo::SetControlChannelPrivilege(uint32_t privilege)
{
    SetRegisterData((uint32)ARV_GVBS_CONTROL_CHANNEL_PRIVILEGE_OFFSET, privilege);
}

uint32_t DeviceInfo::GetPayload()
{
    int width = GetRegisterData((uint32)REG_XML_Width_RegAddr);
    int height = GetRegisterData((uint32)REG_XML_Height_RegAddr);

    return width * height;
}

uint32_t DeviceInfo::GetHeartbeatTimeout()
{
    return GetRegisterData((uint32)ARV_GVBS_HEARTBEAT_TIMEOUT_OFFSET);
}

void DeviceInfo::GetLocalIp(unsigned int& nCurrentIp, unsigned int& nCurrentSubNetMask, unsigned int& nDefultGateWay)
{
	strLocalIP = string("192.168.8.160");
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

const LP_DeviceInfo DeviceInfo::GetDeviceInfo()
{
	return _DeviceInfo;
}

void DeviceInfo::SetDeviceInfo(const LP_DeviceInfo &devInfo)
{
	// 将设备信息写到内存中
	int nVersion = (devInfo.nMinorVer << 16) + devInfo.nMaxorVer;
	SetMemoryData((uint32)ARV_GVBS_VERSION_OFFSET, (&nVersion), 4);
	SetMemoryData((uint32)ARV_GVBS_DEVICE_MODE_OFFSET, (&devInfo.nDeviceMode), 4);
	SetMemoryData((uint32)ARV_GVBS_DEVICE_MAC_ADDRESS_HIGH_OFFSET, (&devInfo.nMacAddrHigh), 4);
	SetMemoryData((uint32)ARV_GVBS_DEVICE_MAC_ADDRESS_LOW_OFFSET, (&devInfo.nMacAddrLow), 4);
	SetMemoryData((uint32)ARV_GVBS_SUPPORTED_IP_CONFIGURATION_OFFSET, (&devInfo.st_gigeInfo.nIpConfigOption), 4);
	SetMemoryData((uint32)ARV_GVBS_CURRENT_IP_CONFIGURATION_OFFSET, (&devInfo.st_gigeInfo.nIpConfigCurrent), 4);
	SetMemoryData((uint32)ARV_GVBS_CURRENT_IP_ADDRESS_OFFSET, (&devInfo.st_gigeInfo.nCurrentIp), 4);
	SetMemoryData((uint32)ARV_GVBS_CURRENT_SUBNET_MASK_OFFSET, (&devInfo.st_gigeInfo.nCurrentSubNetMask), 4);
	SetMemoryData((uint32)ARV_GVBS_CURRENT_GATEWAY_OFFSET, (&devInfo.st_gigeInfo.nDefultGateWay), 4);
	SetMemoryData((uint32)ARV_GVBS_MANUFACTURER_NAME_OFFSET, (&devInfo.st_gigeInfo.cManufacturerName), ARV_GVBS_MANUFACTURER_NAME_SIZE);
	SetMemoryData((uint32)ARV_GVBS_MODEL_NAME_OFFSET, (&devInfo.st_gigeInfo.cModelName), ARV_GVBS_MODEL_NAME_SIZE);
	SetMemoryData((uint32)ARV_GVBS_DEVICE_VERSION_OFFSET, (&devInfo.st_gigeInfo.cDeviceVersion), ARV_GVBS_DEVICE_VERSION_SIZE);
	SetMemoryData((uint32)ARV_GVBS_MANUFACTURER_INFORMATIONS_OFFSET, (&devInfo.st_gigeInfo.cManufacturerSpecificInfo), ARV_GVBS_MANUFACTURER_INFORMATIONS_SIZE);
	SetMemoryData((uint32)ARV_GVBS_SERIAL_NUMBER_OFFSET, (&devInfo.st_gigeInfo.cSerialNumber), ARV_GVBS_SERIAL_NUMBER_SIZE);
	SetMemoryData((uint32)ARV_GVBS_USER_DEFINED_NAME_OFFSET, (&devInfo.st_gigeInfo.cUserDefinedName), ARV_GVBS_USER_DEFINED_NAME_SIZE);
}

bool DeviceInfo::ReadInfobyInIFile(string str)
{
	boost::property_tree::ptree m_pt, ptree_ccinfo;
	read_ini(str, m_pt);

	ptree_ccinfo = m_pt.get_child("INFO_CC");
	_DeviceInfo.nMaxorVer = ptree_ccinfo.get<unsigned short>("MaxorVer");//获取版本号
	_DeviceInfo.nMinorVer = ptree_ccinfo.get<unsigned short>("MinorVer");
	_DeviceInfo.nDeviceMode = ptree_ccinfo.get<unsigned int>("DeviceMode");//设备模式
	//TODO 可加mac地址获取

	boost::property_tree::ptree ptree_gevInfo;
	ptree_gevInfo = m_pt.get_child("INFO_GEV");
	_DeviceInfo.st_gigeInfo.nIpConfigOption = ptree_gevInfo.get<unsigned int>("IpConfigOption");
	_DeviceInfo.st_gigeInfo.nIpConfigCurrent = ptree_gevInfo.get<unsigned int>("IpConfigCurrent");

	memcpy(_DeviceInfo.st_gigeInfo.cManufacturerName, ptree_gevInfo.get<std::string>("ManufacturerName").c_str(), 32);
	memcpy(_DeviceInfo.st_gigeInfo.cModelName, ptree_gevInfo.get<std::string>("ModelName").c_str(), 32);
	memcpy(_DeviceInfo.st_gigeInfo.cDeviceVersion, ptree_gevInfo.get<std::string>("DeviceVersion").c_str(), 32);
	memcpy(_DeviceInfo.st_gigeInfo.cManufacturerSpecificInfo, ptree_gevInfo.get<std::string>("ManufacturerSpecificInfo").c_str(), 48);
	memcpy(_DeviceInfo.st_gigeInfo.cSerialNumber, ptree_gevInfo.get<std::string>("SerialNumber").c_str(), 16);
	memcpy(_DeviceInfo.st_gigeInfo.cUserDefinedName, ptree_gevInfo.get<std::string>("UserDefinedName").c_str(), 16);
	//TODO 可加IP地址 网关等数据获取
	return true;
}

bool DeviceInfo::ReadXMLbyxmlFile(std::string str)
{
	// XML defined register
	SetRegisterData((uint32)REG_XML_AcquisitionStart_RegAddr, 0x0000);
	SetRegisterData((uint32)REG_XML_AcquisitionStop_RegAddr, 0x0001);
	SetRegisterData((uint32)REG_XML_Width_RegAddr, 0x0c00);
	SetRegisterData((uint32)REG_XML_Height_RegAddr, 0x0800);
	SetRegisterData((uint32)REG_XML_OffsetX_RegAddr, 0x0000);
	SetRegisterData((uint32)REG_XML_OffsetY_RegAddr, 0x0000);

	//TODO 需加入文件是否存在判断
	// XML file
	fstream XmlFile;
	XmlFile.open(str.c_str(), ios::in | ios::binary);
	XmlFile.seekg(0, ios::end);
	uint32_t nXmlFileSize = XmlFile.tellg();
	XmlFile.seekg(0, ios::beg);
	if (nXmlFileSize <= GIGE_XML_FILE_MAX_SIZE)
	{
		//xml文件数据大小打包固定格式
		sprintf(pXmlUrl, "Local:%s;%x;%x", str.c_str(), GIGE_REG_MEMORY_SIZE, nXmlFileSize);
		//数据写入到知道内存 供PCSDK读取
		SetMemoryData(uint32(ARV_GVBS_XML_URL_0_OFFSET), pXmlUrl, strlen(pXmlUrl));
		XmlFile.read((char*)m_pMemory + GIGE_REG_MEMORY_SIZE, nXmlFileSize);//读文件
	}
	else
	{
		cout << "Read XML file (" << str << ") fail!!" << endl;
		return false;
	}
	return true;
}

