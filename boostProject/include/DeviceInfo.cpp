#include <stdlib.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "DeviceInfo.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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

unsigned int iptoInt(std::string addrIP) {
	unsigned int ret = 0;
	unsigned char tmp = 0;
	const char *p = addrIP.c_str();
	while (1)
	{
		if (*p != '\0' && *p != '.')
			tmp = tmp * 10 + *p - '0';
		else
		{
			ret = (ret << 8) + tmp;
			if (*p == '\0')
				break;
			tmp = 0;
		}
		p++;
	}
	return ret;
}

std::string InttoIp(unsigned int ipNum)
{
	std::string strIP;
	unsigned char *val = (unsigned char *)&ipNum;
	char *p = (char*)strIP.c_str();
	sprintf(p, "%u.%u.%u.%u", val[3], val[2], val[1], val[1]);
	return strIP;
}


using namespace std;
DeviceInfo::DeviceInfo():m_pMemory(NULL), m_totalMemSize(GIGE_REG_MEMORY_SIZE + GIGE_XML_FILE_MAX_SIZE)
{
	m_pMemory = new unsigned char[m_totalMemSize];
	memset(m_pMemory, 0, m_totalMemSize);

	pXmlUrl = new char[ARV_GVBS_XML_URL_SIZE];

	_strXmlFileName = "E:\\LeaperProject\\ydpanProject\\arv-fake-camera.xml";
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
    SetRegisterData((uint32)ARV_GVBS_CONTROL_CHANNEL_PRIVILEGE_OFFSET, 0);
    SetRegisterData((uint32)ARV_GVBS_HEARTBEAT_TIMEOUT_OFFSET, 30000);
    SetRegisterData((uint32)ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_HIGH_OFFSET, 0);
    SetRegisterData((uint32)ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_LOW_OFFSET, 1000000000);
    SetRegisterData((uint32)ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_OFFSET, 2048);
    SetRegisterData((uint32)ARV_GVBS_N_STREAM_CHANNELS_OFFSET, 1);
	SetRegisterData((uint32_t)0x670,1000);
	SetRegisterData((uint32_t)0xd00, 0);
	SetRegisterData((uint32_t)0xd08, 1000);


	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_WIDTH, 1920);//ARV_FAKE_CAMERA_WIDTH_DEFAULT
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_HEIGHT,1200);// ARV_FAKE_CAMERA_HEIGHT_DEFAULT
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_PIXEL_FORMAT, ARV_PIXEL_FORMAT_MONO_8);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_SENSOR_WIDTH, ARV_FAKE_CAMERA_SENSOR_WIDTH);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_SENSOR_HEIGHT, ARV_FAKE_CAMERA_SENSOR_HEIGHT);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_X_OFFSET, 0);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_Y_OFFSET, 0);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_BINNING_HORIZONTAL, ARV_FAKE_CAMERA_BINNING_HORIZONTAL_DEFAULT);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_BINNING_VERTICAL, ARV_FAKE_CAMERA_BINNING_HORIZONTAL_DEFAULT);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_ACQUISITION, 0);
	
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_ACQUISITION_MODE, 1);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_ACQUISITION_FRAME_PERIOD_US, 1000000.0 / ARV_FAKE_CAMERA_ACQUISITION_FRAME_RATE_DEFAULT);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_EXPOSURE_TIME_US, ARV_FAKE_CAMERA_EXPOSURE_TIME_US_DEFAULT);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_GAIN_RAW, 0);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_GAIN_MODE, 1);
	SetRegisterData((uint32_t)ARV_FAKE_CAMERA_REGISTER_TEST, ARV_FAKE_CAMERA_TEST_REGISTER_DEFAULT);
	
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
	if (Data == NULL)
		return false;

	boost::mutex::scoped_lock lock(m_udpMutex);
    if (MemAddr <= m_totalMemSize)
    {
        uint8_t *pMemdata = m_pMemory + MemAddr;
		if(pMemdata)
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
	if (Data == NULL)
		return false;
	boost::mutex::scoped_lock lock(m_udpMutex);
    if (MemAddr <= m_totalMemSize)
    {
        uint8_t *pMemAddr = m_pMemory + MemAddr;
		if(pMemAddr)
			memcpy((unsigned char*)pMemAddr, (unsigned char*)Data, Count);
    }
    else
    {
		return false;
    }

    return true;
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

uint32_t DeviceInfo::GetSCP0()
{
	uint32_t nSCP0;
	nSCP0 = GetRegisterData(ARV_GVBS_STREAM_CHANNEL_0_PACKET_DELAY_OFFSET);
	return nSCP0;
}

uint32_t DeviceInfo::GetStream_Channel_0_Packet_Size()
{// xihua
	uint32_t nSCPS0;
	nSCPS0 = GetRegisterData(ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_OFFSET);
	uint32_t packSizeMask = nSCPS0 & ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_MASK;
	uint32_t packSizePos = nSCPS0 & ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_POS;
	uint32_t packSizeBig = nSCPS0 & ARV_GVBS_STREAM_CHANNEL_0_PACKET_BIG_ENDIAN;
	uint32_t packSizedoNot = nSCPS0 & ARV_GVBS_STREAM_CHANNEL_0_PACKET_DO_NOT_FRAGMENT;
	uint32_t packSizeFiRE = nSCPS0 & ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_FIRE_TEST;

	return packSizeMask;
}

uint32_t DeviceInfo::GetPersistenDefaultGateway()
{
	uint32_t nGateway;
	nGateway = GetRegisterData(ARV_GVBS_PERSISTENT_GATEWAY_0_OFFSET);
	return nGateway;
}

uint32_t DeviceInfo::GetPersistentsubnetMask()
{
	uint32_t nSubNetMask;
	nSubNetMask = GetRegisterData(ARV_GVBS_PERSISTENT_SUBNET_MASK_0_OFFSET);
	return nSubNetMask;
}

uint32_t DeviceInfo::GetPersistenIPAddress()
{
	uint32_t nIPAddress;
	nIPAddress = GetRegisterData(ARV_GVBS_PERSISTENT_IP_ADDRESS_0_OFFSET);
	return nIPAddress;
}

uint32_t DeviceInfo::GetCurrentDefaultGateway()
{
	uint32_t nGateway;
	nGateway = GetRegisterData(ARV_GVBS_CURRENT_GATEWAY_OFFSET);
	return nGateway;
}

uint32_t DeviceInfo::GetCurrentsubnetMask()
{
	uint32_t nSubNetMask;
	nSubNetMask = GetRegisterData(ARV_GVBS_CURRENT_SUBNET_MASK_OFFSET);
	return nSubNetMask;
}

uint32_t DeviceInfo::GetCurrentIPAddress()
{
	uint32_t nIPAddress;
	nIPAddress = GetRegisterData(ARV_GVBS_CURRENT_IP_ADDRESS_OFFSET);
	return nIPAddress;
}

uint32_t DeviceInfo::GetCurrentIPConfig()
{//
	uint32_t nIPConfig;
	nIPConfig = GetRegisterData(ARV_GVBS_CURRENT_IP_CONFIGURATION_OFFSET);
	return nIPConfig;
}

uint32_t DeviceInfo::GetSupportIPConfiguration0()
{
	uint32_t nSIC0;
	nSIC0 = GetRegisterData(ARV_GVBS_SUPPORTED_IP_CONFIGURATION_OFFSET);
	return nSIC0;
}

uint32_t DeviceInfo::GetStreamChannelCapability()
{
	uint32_t nSCC;
	nSCC = GetRegisterData(0x92c);
	return nSCC;
}

uint32_t DeviceInfo::GetDeviceMacAddressHigh0()
{
	uint32_t nDAH0;
	nDAH0 = GetRegisterData(ARV_GVBS_DEVICE_MAC_ADDRESS_HIGH_OFFSET);
	return nDAH0;
}

uint32_t DeviceInfo::GetDeviceMacAddressLow0()
{
	uint32_t nDAL0;
	nDAL0 = GetRegisterData(ARV_GVBS_DEVICE_MAC_ADDRESS_LOW_OFFSET);
	return nDAL0;
}

uint32_t DeviceInfo::GetGVCPCapability()
{//
	uint32_t nGVCPC;
	nGVCPC = GetRegisterData(ARV_GVBS_GVCP_CAPABILITY_OFFSET);
	return nGVCPC;
}

uint32_t DeviceInfo::GetHeartbeatTimeout()
{
	return GetRegisterData((uint32)ARV_GVBS_HEARTBEAT_TIMEOUT_OFFSET);
}

uint32_t DeviceInfo::GetTimestampTickFrequencyHigh()
{
	uint32_t nTTFH;
	nTTFH = GetRegisterData(ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_HIGH_OFFSET);
	return nTTFH;
}

uint32_t DeviceInfo::GetTimestampTickFrequencyLow()
{
	uint32_t nTTFL;
	nTTFL = GetRegisterData(ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_LOW_OFFSET);
	return nTTFL;
}
uint32_t DeviceInfo::GetTimestampControl()
{
	uint32_t nTimecontrol;
	nTimecontrol = GetRegisterData(ARV_GVBS_TIMESTAMP_CONTROL_OFFSET);
	return nTimecontrol;
}

uint32_t DeviceInfo::GetTimestampLatchedValueHigh()
{
	uint32_t nTLVH;
	nTLVH = GetRegisterData(ARV_GVBS_TIMESTAMP_LATCHED_VALUE_HIGH_OFFSET);
	return nTLVH;
}

uint32_t DeviceInfo::GetTimestampLatchedValueLow()
{
	uint32_t nTLVL;
	nTLVL = GetRegisterData(ARV_GVBS_TIMESTAMP_LATCHED_VALUE_LOW_OFFSET);
	return nTLVL;
}

uint32_t DeviceInfo::GetGVCPConfiguration()
{
	uint32_t nGVCPConfig;
	nGVCPConfig = GetRegisterData(0x954);
	return nGVCPConfig;
}

uint32_t DeviceInfo::GetVersion()
{
	uint32_t nVersion;
	nVersion = GetRegisterData(ARV_GVBS_VERSION_OFFSET);
	return nVersion;
}

uint32_t DeviceInfo::GetLinkSpeed0()
{
	uint32_t nSpeed;
	nSpeed = GetRegisterData(0x670);
	return nSpeed;
}

uint32_t DeviceInfo::GetNumberOfNetworkInterfaces()
{
	uint32_t nNum;
	nNum = GetRegisterData(ARV_GVBS_N_NETWORK_INTERFACES_OFFSET);
	return nNum;
}

uint32_t DeviceInfo::GetUserDefineName()
{//!!!
	uint32_t sdata;
	GetMemoryData(ARV_GVBS_USER_DEFINED_NAME_OFFSET, &sdata, ARV_GVBS_USER_DEFINED_NAME_SIZE);
	return sdata;
}

uint32_t DeviceInfo::GetSerialNumber()
{
	uint32_t sSerialNum;
	GetMemoryData(ARV_GVBS_SERIAL_NUMBER_OFFSET, &sSerialNum, ARV_GVBS_SERIAL_NUMBER_SIZE);
	return sSerialNum;
}

uint32_t DeviceInfo::GetDeviceVersion()
{
	uint32_t sDeviceVersion;
	GetMemoryData(ARV_GVBS_DEVICE_VERSION_OFFSET, &sDeviceVersion, ARV_GVBS_DEVICE_VERSION_SIZE);
	return sDeviceVersion;
}

uint32_t DeviceInfo::GetManufactureSpecificInfo()
{
	uint32_t nInfo;
	GetMemoryData(ARV_GVBS_MANUFACTURER_INFORMATIONS_OFFSET, &nInfo, ARV_GVBS_MANUFACTURER_INFORMATIONS_SIZE);
	return nInfo;
}

uint32_t DeviceInfo::GetModelName()
{
	uint32_t nModelName;
	GetMemoryData(ARV_GVBS_MODEL_NAME_OFFSET, &nModelName, ARV_GVBS_MODEL_NAME_SIZE);
	return nModelName;
}

uint32_t DeviceInfo::GetManufactureName()
{
	uint32_t nMoanufactureName;
	GetMemoryData(ARV_GVBS_MANUFACTURER_NAME_OFFSET, &nMoanufactureName, ARV_GVBS_MANUFACTURER_NAME_SIZE);
	return nMoanufactureName;
}

uint32_t DeviceInfo::GetDeviceMode()
{
	uint32_t nDeviceMode;
	nDeviceMode = GetRegisterData(ARV_GVBS_DEVICE_MODE_OFFSET);
	return nDeviceMode;
}

uint32_t DeviceInfo::GetNumofStreamChannels()
{
	uint32_t nNumStreamChannels;
	nNumStreamChannels = GetRegisterData(ARV_GVBS_N_STREAM_CHANNELS_OFFSET);
	return nNumStreamChannels;
}

uint32_t DeviceInfo::GetDestinationAddress()
{
	uint32_t nAddress;
	nAddress = GetRegisterData(ARV_GVBS_STREAM_CHANNEL_0_IP_ADDRESS_OFFSET);
	return nAddress;
}

uint32_t DeviceInfo::GetDestinationPort()
{
	uint32_t nDestinationPort;
	nDestinationPort = GetRegisterData(ARV_GVBS_STREAM_CHANNEL_0_PORT_OFFSET);
	return nDestinationPort;
}

// uint32_t DeviceInfo::GetControlChannelPrivilege()
// {
// 	uint32_t nCCP;
// 	nCCP = GetRegisterData(0x930);
// 	return nCCP;
// 	//recv status 0x8003
// }

uint32_t DeviceInfo::GetAcquisitionStatus()
{
	return GetRegisterData((uint32)ARV_FAKE_CAMERA_REGISTER_ACQUISITION);
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
	int width = GetRegisterData((uint32)ARV_FAKE_CAMERA_REGISTER_WIDTH);
	int height = GetRegisterData((uint32)ARV_FAKE_CAMERA_REGISTER_HEIGHT);

	return width * height;
}

uint32_t DeviceInfo::GetDeviceLinkSpeed()
{
	uint32_t nVal = GetRegisterData(ARV_GVBS_DEVICE_LINK_SPEED_0_OFFSET);
	return nVal;
}

void DeviceInfo::SetAcquisitionStatus(uint32_t val)
{
	SetRegisterData((uint32)ARV_FAKE_CAMERA_REGISTER_ACQUISITION,val);
}

uint32_t DeviceInfo::GetTrigerModel()
{
	return GetRegisterData(ARV_FAKE_CAMERA_REGISTER_TRIGGER_MODE);
}

uint32_t DeviceInfo::GetAcquisitionFramePeridUS()
{
	return GetRegisterData(ARV_FAKE_CAMERA_REGISTER_ACQUISITION_FRAME_PERIOD_US);
}

uint64_t DeviceInfo::Get_Sleep_time_for_next_frame(uint64_t next_timestamp)
{
	uint64_t time_us;
	uint64_t sleep_time_us;
	uint64_t frame_period_time_us;
	if (GetTrigerModel() == 1)
		frame_period_time_us = 1000000 / triger_frequency;
	else
		frame_period_time_us = (uint64_t)GetAcquisitionFramePeridUS();
	if (frame_period_time_us == 0)
		frame_period_time_us = 1000000;

	time_us = getcurrentTime();//获取系统时间
	sleep_time_us = frame_period_time_us - (time_us%frame_period_time_us);

	next_timestamp = time_us + sleep_time_us;
	return sleep_time_us;
}

int DeviceInfo::getcurrentTime()
{
	//const boost::posix_time::ptime time_now = boost::posix_time::microsec_clock::local_time();
	const boost::posix_time::time_duration td = boost::get_system_time().time_of_day();
	int hh = td.hours();
	int mm = td.minutes();
	int ss = td.seconds();
	int ms = td.total_microseconds() - ((hh * 3600 + mm * 60 + ss) * 1000);
	
	return ms;
}

void DeviceInfo::UpdateCtroller_time()
{
	controller_time = getcurrentTime();
}

