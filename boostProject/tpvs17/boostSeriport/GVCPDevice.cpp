#include "GVCPDevice.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

GVCPDevice::GVCPDevice()
{
}


GVCPDevice::~GVCPDevice()
{
}
// void GetLocalMac(std::string straddress, unsigned int& nMacAddrHigh, unsigned int& nMacAddrLow)
// {
// 	{
// 		long long nMacAddr;
// 		memcpy((unsigned char*)&nMacAddr, (unsigned char*)straddress.c_str(), straddress.size());
// 		nMacAddrHigh = (nMacAddr & 0x0000000000ffff);
// 		nMacAddrHigh = ntohl(nMacAddrHigh);
// 		nMacAddrLow = (nMacAddr & 0xffffffffff0000) >> 16;
// 		nMacAddrLow = ntohl(nMacAddrLow);
// 	}
// }

// void GetLocalIp(void* pInfo, unsigned int& nCurrentIp, unsigned int& nCurrentSubNetMask, unsigned int& nDefultGateWay)
// {
// 	{
// 		//((IP_ADAPTER_INFO*))pInfo->IpAddressList.IpAddress.String = string("192.168.8.152").c_str();
// 		char strIP[16];
// 		string st = string("192.168.242.152");
// 		for (int i = 0; i < st.size(); i++)
// 			strIP[i] = st[i];
// 		nCurrentIp = inet_addr(strIP/*((IP_ADAPTER_INFO*)pInfo)->IpAddressList.IpAddress.String*/);
// 		//nCurrentIp = inet_addr(((IP_ADAPTER_INFO*)pInfo)->IpAddressList.IpAddress.String);
// 		nCurrentSubNetMask = inet_addr(((IP_ADAPTER_INFO*)pInfo)->IpAddressList.IpMask.String);
// 		nDefultGateWay = inet_addr(((IP_ADAPTER_INFO*)pInfo)->GatewayList.IpAddress.String);
// 		nCurrentIp = ntohl(nCurrentIp);
// 		nCurrentSubNetMask = ntohl(nCurrentSubNetMask);
// 		nDefultGateWay = ntohl(nDefultGateWay);
// 	}
// }

void GVCPDevice::InitDevice()
{
	readDeViceInfo();
	m_pUdpServer = boost::make_shared<UDPService>(MV_GVCP_PUBLIC_PORT);
	m_pUdpServer->setCallbackFunc(boost::bind(&GVCPDevice::handMsgCallBack, this, _1));
	m_pUdpServer->startThread();
}

void GVCPDevice::readDeViceInfo()
{

}

void GVCPDevice::handMsgCallBack(tagUdpData tagData)
{// TODO 处理回调数据
	unsigned int nlen = MV_GVCP_MAX_MSG_LEN;
	if (nlen < tagData._byteData->size())
		return;
	if (nlen <= 0)
		return;
	std::string strAdr = tagData.fromPoint.address().to_string();
	if (strAdr != "192.168.8.254")
		//return;
	std::string strAdressPort = tagData.fromPoint.address().to_string() + ":" + to_string(tagData.fromPoint.port());
	unsigned char *pData = tagData._byteData->data();
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)(pData);
	_pCmdHdr = pCmdHdr;
	if (pCmdHdr->cKeyValue == MV_GVCP_CMD_KEY_VALUE)
	{
		switch (ntohs(pCmdHdr->nCommand))
		{
		case MV_GEV_DISCOVERY_CMD:
		{
			DiscoveryAck(tagData);
			break;
		}
		case MV_GEV_READREG_CMD:
		{
			int a = 0;
			break;
		}
		case MV_GEV_WRITEREG_CMD:
		{
			int a = 0;
			break;
		}
		case MV_GEV_READMEM_CMD:
		{
			int a = 0;
			break;
		}
		case MV_GEV_WRITEMEM_CMD:
		{
			int a = 0;
			break;
		}
		default:
			std::string str = to_string(pCmdHdr->nCommand);
			break;
		}
	}

}

int GVCPDevice::DiscoveryAck(tagUdpData &tagdata)
{
	int nRet = MV_OK;

	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)(pData);
	ACK_MSG_HEADER* pAckHdr = (ACK_MSG_HEADER*)cSendData;
	pAckHdr->nStatus = htons(MV_GEV_STATUS_SUCCESS);
	pAckHdr->nAckMsgValue = htons(MV_GEV_DISCOVERY_ACK);
	pAckHdr->nLength = htons(sizeof(DISCOVERY_ACK_MSG));
	pAckHdr->nAckId = htons(ntohs(pCmdHdr->nReqId));

	if (_nLastAckId != ntohs(pCmdHdr->nReqId))
	{
		// Update last ack_id
		_nLastAckId = ntohs(pCmdHdr->nReqId);
	}

	DISCOVERY_ACK_MSG* pAckMsg = (DISCOVERY_ACK_MSG*)(cSendData + sizeof(ACK_MSG_HEADER));
	m_deveiceInfo = m_device.GetDeviceInfo();
	pAckMsg->nMajorVer = htons(m_deveiceInfo.nMajorVer);
	pAckMsg->nMinorVer = htons(m_deveiceInfo.nMinorVer);
	pAckMsg->nDeviceMode = htonl(m_deveiceInfo.nDeviceMode);
	pAckMsg->nMacAddrHigh = htonl(m_deveiceInfo.nMacAddrHigh) & 0xffff;
	pAckMsg->nMacAddrLow = htonl(m_deveiceInfo.nMacAddrLow);
	pAckMsg->nIpCfgOption = htonl(m_deveiceInfo.stGigEInfo.nIpCfgOption);
	pAckMsg->nIpCfgCurrent = htonl(m_deveiceInfo.stGigEInfo.nIpCfgCurrent);
	pAckMsg->nCurrentIp = htonl(m_deveiceInfo.stGigEInfo.nCurrentIp);
	pAckMsg->nCurrentSubNetMask = htonl(m_deveiceInfo.stGigEInfo.nCurrentSubNetMask);
	pAckMsg->nDefultGateWay = htonl(m_deveiceInfo.stGigEInfo.nDefultGateWay);

	memcpy(pAckMsg->chManufacturerName, m_deveiceInfo.stGigEInfo.chManufacturerName, 32);
	memcpy(pAckMsg->chModelName, m_deveiceInfo.stGigEInfo.chModelName, 32);
	memcpy(pAckMsg->chDeviceVersion, m_deveiceInfo.stGigEInfo.chDeviceVersion, 32);
	memcpy(pAckMsg->chManufacturerSpecificInfo, m_deveiceInfo.stGigEInfo.chManufacturerSpecificInfo, 48);
	memcpy(pAckMsg->chSerialNumber, m_deveiceInfo.stGigEInfo.chSerialNumber, 16);
	memcpy(pAckMsg->chUserDefinedName, m_deveiceInfo.stGigEInfo.chUserDefinedName, 16);

	try
	{
		size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->nLength);

		tagUdpData m_writeData;
		m_writeData.fromPoint = tagdata.fromPoint;
		m_writeData._byteData = boost::shared_ptr<ByteVector>( new ByteVector);
		m_writeData._byteData->resize(nLen, 0);
		unsigned char *pData = m_writeData._byteData->data();
		memcpy(pData, cSendData, nLen);
		std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

		m_pUdpServer->writeRaw(m_writeData);
	}
	catch (std::exception &e)
	{
		nRet = MV_E_NETER;
		return nRet;
	}

	return nRet;
	return 0;
}

int GVCPDevice::ReadRegAck()
{
	return 0;
}

int GVCPDevice::WriteRegAck()
{
	return 0;
}

int GVCPDevice::ReadMemAck()
{
	return 0;
}

int GVCPDevice::WriteMemAck()
{
	return 0;
}
