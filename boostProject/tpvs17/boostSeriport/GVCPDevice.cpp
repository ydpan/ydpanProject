#include "GVCPDevice.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
/*
htons: short 类型 主机到网络端的排序
htonl: unsigned int 类型 主机到网络端的排序

ntohs: short 类型  网络到主机端的排序
ntohl: unsigned int 类型 网络到主机端的排序
*/
GVCPDevice::GVCPDevice()
{
}


GVCPDevice::~GVCPDevice()
{
}

void GVCPDevice::InitDevice()
{
	readDeViceInfo();
	m_pUdpServer = boost::make_shared<UDPService>("127.0.0.1",ARV_GVCP_PORT);
	m_pUdpServer->setCallbackFunc(boost::bind(&GVCPDevice::handMsgCallBack, this, _1));
	m_pUdpServer->startThread();
}

void GVCPDevice::readDeViceInfo()
{
	m_device.Init();
}

void GVCPDevice::handMsgCallBack(tagUdpData tagData)
{// TODO 处理回调数据
	unsigned int nlen = ARV_GVCP_MAX_MSG_LEN;
	if (nlen < tagData._byteData->size())
		return;
	if (nlen <= 0)
		return;
	std::string strAdr = tagData.fromPoint.address().to_string();
	if (strAdr != "192.168.8.254")
		;
		//return;
	std::string strAdressPort = tagData.fromPoint.address().to_string() + ":" + to_string(tagData.fromPoint.port());
	unsigned char *pData = tagData._byteData->data();
	ArvGvcpPacket* packet = (ArvGvcpPacket*)(pData);
	if (packet == NULL)
		return;
	guint16 packet_type = arv_gvcp_packet_get_packet_type(packet);
	unsigned char keyVal = packet_type >> 8;
	if (keyVal != 0x42)
		return;

	switch (ntohs(packet->header.command))
	{
		case ARV_GVCP_COMMAND_DISCOVERY_CMD://discover_cmd
		{
			DiscoveryAck(tagData);
			break;
		}
		case ARV_GVCP_COMMAND_READ_MEMORY_CMD://读内存
		{
			ReadMemoryAck(tagData);
			break;
		}
		case ARV_GVCP_COMMAND_WRITE_MEMORY_CMD://写内存
		{
			WriteMemoryAck(tagData);
			break;
		}
		case ARV_GVCP_COMMAND_READ_REGISTER_CMD://读寄存器
		{
			ReadRegisterAck(tagData);
			break;
		}
		case ARV_GVCP_COMMAND_WRITE_REGISTER_CMD://写寄存器
		{
			WriteRegisterAck(tagData);
			break;
		}
		case ARV_GVCP_COMMAND_FOCUSIP_CMD:
		{
			FocusIPAck(tagData);
			break;
		}
		default:
			std::string str = to_string(packet->header.command);
			break;
	}
}

void GVCPDevice::DiscoveryAck(tagUdpData &tagdata)
{
	char cSendData[ARV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	ArvGvcpPacket* pCmdHdr = (ArvGvcpPacket*)(pData);
	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;
	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_DISCOVERY_ACK);
	pAckHdr->nlength = htons(sizeof(DISCOVERY_ACK_MSG));
	pAckHdr->nRequesd_Id = htons(ntohs(pCmdHdr->header.nRequesd_Id));

	if (_nLastAckId != ntohs(pCmdHdr->header.nRequesd_Id))
	{
		_nLastAckId = ntohs(pCmdHdr->header.nRequesd_Id);
	}

	DISCOVERY_ACK_MSG* pAckMsg = (DISCOVERY_ACK_MSG*)(cSendData + sizeof(ArvGvcpHeader));
	LP_DeviceInfo  m_deveiceInfo = m_device.GetDeviceInfo();
	pAckMsg->nMajorVer = htons(m_deveiceInfo.nMaxorVer);
	pAckMsg->nMinorVer = htons(m_deveiceInfo.nMinorVer);
	pAckMsg->nDeviceMode = htonl(m_deveiceInfo.nDeviceMode);
	pAckMsg->nMacAddrHigh = htonl(m_deveiceInfo.nMacAddrHigh) & 0xffff;
	pAckMsg->nMacAddrLow = htonl(m_deveiceInfo.nMacAddrLow);
	pAckMsg->nIpCfgOption = htonl(m_deveiceInfo.st_gigeInfo.nIpConfigOption);
	pAckMsg->nIpCfgCurrent = htonl(m_deveiceInfo.st_gigeInfo.nIpConfigCurrent);
	pAckMsg->nCurrentIp = htonl(m_deveiceInfo.st_gigeInfo.nCurrentIp);
	pAckMsg->nCurrentSubNetMask = htonl(m_deveiceInfo.st_gigeInfo.nCurrentSubNetMask);
	pAckMsg->nDefultGateWay = htonl(m_deveiceInfo.st_gigeInfo.nDefultGateWay);

	memcpy(pAckMsg->chManufacturerName, m_deveiceInfo.st_gigeInfo.cManufacturerName, 32);
	memcpy(pAckMsg->chModelName, m_deveiceInfo.st_gigeInfo.cModelName, 32);
	memcpy(pAckMsg->chDeviceVersion, m_deveiceInfo.st_gigeInfo.cDeviceVersion, 32);
	memcpy(pAckMsg->chManufacturerSpecificInfo, m_deveiceInfo.st_gigeInfo.cManufacturerSpecificInfo, 48);
	memcpy(pAckMsg->chSerialNumber, m_deveiceInfo.st_gigeInfo.cSerialNumber, 16);
	memcpy(pAckMsg->chUserDefinedName, m_deveiceInfo.st_gigeInfo.cUserDefinedName, 16);

	try
	{
		size_t nLen = sizeof(ArvGvcpHeader) + ntohs(pAckHdr->nlength);

		tagUdpData m_writeData;
		m_writeData.fromPoint = tagdata.fromPoint;
		m_writeData._byteData = boost::make_shared<ByteVector>();// boost::shared_ptr<ByteVector>(new ByteVector);
		m_writeData._byteData->resize(nLen, 0);
		unsigned char *pData = m_writeData._byteData->data();
		memcpy(pData, cSendData, nLen);
		std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

		m_pUdpServer->writeRaw(m_writeData);
	}
	catch (std::exception &e)
	{
		
	}
}
/*读寄存器*/
void GVCPDevice::ReadRegisterAck(tagUdpData &tagdata)
{
	char cSendData[ARV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pRecvData = tagdata._byteData->data();
	ArvGvcpPacket* pRecvPacket = (ArvGvcpPacket*)(pRecvData);
	ArvGvcpHeader* pAckHeader = (ArvGvcpHeader*)cSendData;
	pAckHeader->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHeader->command = htons(ARV_GVCP_COMMAND_READ_REGISTER_ACK);
	pAckHeader->nRequesd_Id = htons(ntohs(pRecvPacket->header.nRequesd_Id));

// 	if (_nLastAckId != ntohs(pRecvPacket->header.nRequesd_Id))
// 	{
// 		_nLastAckId = ntohs(pRecvPacket->header.nRequesd_Id);
// 	}
	unsigned short nlength = ntohs(pRecvPacket->header.nlength);
	unsigned int *pRegAddress = (unsigned int *)pRecvPacket->bodydata;
	unsigned int *pMomeryData = (unsigned int *)(cSendData + sizeof(ArvGvcpHeader));

	int nCount = nlength / sizeof(unsigned int);
	for (int i = 0; i <= nCount; i++) {
		unsigned int nRegAddr = *(pRegAddress + i * sizeof(unsigned int));
		nRegAddr = ntohl(nRegAddr);
		unsigned int *pGetData = pMomeryData + i * sizeof(unsigned int);
		unsigned int nData = 0;
		m_device.GetRegisterData(nRegAddr, nData);
		*pGetData = htonl(nData);
	}
// 	guint32 register_address = 0;
// 	arv_gvcp_packet_get_read_register_cmd_infos(pRecvPacket, &register_address);
// 	guint32 data;
// 	m_device.GetRegisterData(register_address, data);
	/*
	TODO : camera read register
	
	根据地址获取数据 
	*/
	pAckHeader->nlength = htons(ntohs(pRecvPacket->header.nlength));
	size_t nLen = sizeof(ArvGvcpHeader) + ntohs(pAckHeader->nlength);

	tagUdpData m_writeData;
	m_writeData.fromPoint = tagdata.fromPoint;
	m_writeData._byteData = boost::shared_ptr<ByteVector>(new ByteVector);
	m_writeData._byteData->resize(nLen, 0);
	unsigned char *pSendData = m_writeData._byteData->data();
	memcpy(pSendData, cSendData, nLen);
	std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

	m_pUdpServer->writeRaw(m_writeData);

}

void GVCPDevice::WriteRegisterAck(tagUdpData &tagdata)
{
	char cSendData[ARV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pRecvData = tagdata._byteData->data();
	ArvGvcpPacket* pRecvPacket = (ArvGvcpPacket*)(pRecvData);
	ArvGvcpHeader* pAckHeader = (ArvGvcpHeader*)cSendData;
	pAckHeader->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHeader->command = htons(ARV_GVCP_COMMAND_WRITE_REGISTER_ACK);
	pAckHeader->nRequesd_Id = htons(ntohs(pRecvPacket->header.nRequesd_Id));

// 	if (_nLastAckId != ntohs(pRecvPacket->header.nRequesd_Id))
// 	{
// 		_nLastAckId = ntohs(pRecvPacket->header.nRequesd_Id);
// 	}
	unsigned short payload_size = ntohs(pRecvPacket->header.nlength);
	int nCount = payload_size / sizeof(WRITEREG_CMD_MSG);
	int nIndex = nCount;
	WRITEREG_CMD_MSG* pCurCmdMsg = (WRITEREG_CMD_MSG*)(pRecvPacket->bodydata);
	for (unsigned int i = 0; i <= nCount; i++)
	{
		pCurCmdMsg = (WRITEREG_CMD_MSG*)pCurCmdMsg + i * sizeof(WRITEREG_CMD_MSG);
		unsigned int RegData = ntohl(pCurCmdMsg->nRegData);
		unsigned int nRegAddress = ntohl(pCurCmdMsg->nRegAddress);
		if (m_device.SetRegisterData(nRegAddress, RegData) == false)
		{
			nIndex = i;
			break;
		}
	}
	//arv_gvcp_packet_get_write_register_cmd_infos(pRecvPacket, &register_address,&register_value);

	tag_WriteReg_ACK *pAck = (tag_WriteReg_ACK *)(cSendData + sizeof(ArvGvcpHeader));
	pAck->nReserved = 0;
	pAck->nIndex = htons(nIndex);//写成功标识符
	pAckHeader->nlength = htons(sizeof(tag_WriteReg_ACK));
	size_t nLen = sizeof(ArvGvcpHeader) + ntohs(pAckHeader->nlength);

	tagUdpData m_writeData;
	m_writeData.fromPoint = tagdata.fromPoint;
	m_writeData._byteData = boost::shared_ptr<ByteVector>(new ByteVector);
	m_writeData._byteData->resize(nLen, 0);
	unsigned char *pSendData = m_writeData._byteData->data();
	memcpy(pSendData, cSendData, nLen);
	std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

	m_pUdpServer->writeRaw(m_writeData);
}

void GVCPDevice::ReadMemoryAck(tagUdpData &tagdata)
{
	char cSendData[ARV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pRecvData = tagdata._byteData->data();
	ArvGvcpPacket* pRecvPacket = (ArvGvcpPacket*)(pRecvData);
	
// 	if (_nLastAckId != ntohs(pRecvPacket->header.nRequesd_Id))
// 	{
// 		_nLastAckId = ntohs(pRecvPacket->header.nRequesd_Id);
// 	}
	//取出数据体部分
	tag_ReadMem_CMD* pRecvBody =(tag_ReadMem_CMD*)pRecvPacket->bodydata;
	//获取网络数据 转换
	unsigned int register_address = ntohl(pRecvBody->nMemAddress);
	unsigned short block_size = ntohs(pRecvBody->count);

	tag_Readmem_ACK *pReadMem = (tag_Readmem_ACK *)(cSendData + sizeof(ArvGvcpHeader));
	
	m_device.GetMemoryData(register_address, (unsigned char *)pReadMem->cReadMemData, block_size);
	
	//发送前 数据转换
	pReadMem->nMemAddress = htonl(register_address);

	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;
	pAckHdr->nlength = htons(sizeof(pReadMem->nMemAddress) + block_size);
	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_READ_MEMORY_ACK);
	pAckHdr->nRequesd_Id = htons(ntohs(pRecvPacket->header.nRequesd_Id));

	size_t nLen = sizeof(ArvGvcpHeader) + ntohs(pAckHdr->nlength);
	tagUdpData m_writeData;
	m_writeData.fromPoint = tagdata.fromPoint;
	m_writeData._byteData = boost::shared_ptr<ByteVector>(new ByteVector);
	m_writeData._byteData->resize(nLen, 0);
	unsigned char *pSendData = m_writeData._byteData->data();
	memcpy(pSendData, cSendData, nLen);
	std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

	m_pUdpServer->writeRaw(m_writeData);
}

void GVCPDevice::WriteMemoryAck(tagUdpData &tagdata)
{
	char cSendData[ARV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pRecvData = tagdata._byteData->data();
	ArvGvcpPacket* pRecvPacket = (ArvGvcpPacket*)(pRecvData);
	ArvGvcpHeader* pAckPacket = (ArvGvcpHeader*)cSendData;

	pAckPacket->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckPacket->command = htons(ARV_GVCP_COMMAND_WRITE_MEMORY_ACK);
	pAckPacket->nRequesd_Id = htons(ntohs(pRecvPacket->header.nRequesd_Id));

// 	if (_nLastAckId != ntohs(pRecvPacket->header.nRequesd_Id))
// 	{
// 		_nLastAckId = ntohs(pRecvPacket->header.nRequesd_Id);
// 	}

	tag_WriteMem_CMD* pWriteData = (tag_WriteMem_CMD*)pRecvPacket->bodydata;
	unsigned int block_address = ntohl(pWriteData->nMemAddress);//数据的起始地址
	//pWriteData->cWriteMemData;//需要写入的数据
	unsigned short block_size = ntohs(pRecvPacket->header.nlength) - sizeof(pWriteData->nMemAddress);//数据大小  对齐
	m_device.SetMemoryData(block_address, (unsigned char *)pWriteData->cWriteMemData, block_size);

	tag_WriteMem_ACK *pAckData = (tag_WriteMem_ACK *)(cSendData + sizeof(ArvGvcpHeader));
	pAckData->nIndex = htons(ntohs(pRecvPacket->header.nlength) - sizeof(char*));
	pAckData->nReserved = 0;
	pAckPacket->nlength = htons(sizeof(tag_WriteMem_ACK));

	size_t nLen = sizeof(ArvGvcpHeader) + ntohs(pAckPacket->nlength);

	tagUdpData m_writeData;
	m_writeData.fromPoint = tagdata.fromPoint;
	m_writeData._byteData = boost::shared_ptr<ByteVector>(new ByteVector);
	m_writeData._byteData->resize(nLen, 0);
	unsigned char *pSendData = m_writeData._byteData->data();
	memcpy(pSendData, cSendData, nLen);
	std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

	m_pUdpServer->writeRaw(m_writeData);
}

void GVCPDevice::FocusIPAck(tagUdpData &tagdata)
{
	char cSendData[ARV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	ArvGvcpPacket* packet = (ArvGvcpPacket*)(pData);
	/*
	fOCUSIP 
	
	*/
	Arv_FocusIP_Msg* pFocusIP = (Arv_FocusIP_Msg*)packet->bodydata;
	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;
	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_FOCUSIP_ACK);
	pAckHdr->nlength = htons(sizeof(DISCOVERY_ACK_MSG));
	pAckHdr->nRequesd_Id = htons(ntohs(packet->header.nRequesd_Id));

	if (_nLastAckId != ntohs(packet->header.nRequesd_Id))
	{
		_nLastAckId = ntohs(packet->header.nRequesd_Id);
	}
	guint32 block_address = 0;
	guint32 block_size = 0;
	arv_gvcp_packet_get_write_memory_cmd_infos(packet, &block_address, &block_size);

	/*
	TODO : camera write memory

	*/

	size_t nLen = sizeof(ArvGvcpHeader) + ntohs(pAckHdr->nlength);

	tagUdpData m_writeData;
	m_writeData.fromPoint = tagdata.fromPoint;
	m_writeData._byteData = boost::shared_ptr<ByteVector>(new ByteVector);
	m_writeData._byteData->resize(nLen, 0);
	unsigned char *pSendData = m_writeData._byteData->data();
	memcpy(pSendData, cSendData, nLen);
	std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

	m_pUdpServer->writeRaw(m_writeData);
}
