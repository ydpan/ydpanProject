#include "GVCPDevice.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

GVCPDevice::GVCPDevice()
{
}


GVCPDevice::~GVCPDevice()
{
}

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
	pAckHdr->size = htons(sizeof(DISCOVERY_ACK_MSG));
	pAckHdr->id = htons(ntohs(pCmdHdr->header.id));

	if (_nLastAckId != ntohs(pCmdHdr->header.id))
	{
		_nLastAckId = ntohs(pCmdHdr->header.id);
	}

	DISCOVERY_ACK_MSG* pAckMsg = (DISCOVERY_ACK_MSG*)(cSendData + sizeof(ArvGvcpHeader));
	MV_CC_DEVICE_INFO  m_deveiceInfo = m_device.GetDeviceInfo();
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
		size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->size);

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
		
	}
}
/*读寄存器*/
void GVCPDevice::ReadRegisterAck(tagUdpData &tagdata)
{
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	ArvGvcpPacket* packet = (ArvGvcpPacket*)(pData);
	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;
	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_READ_REGISTER_ACK);
	pAckHdr->id = htons(ntohs(packet->header.id));

	if (_nLastAckId != ntohs(packet->header.id))
	{
		_nLastAckId = ntohs(packet->header.id);
	}

	guint32 register_address = 0;
	arv_gvcp_packet_get_read_register_cmd_infos(packet, &register_address);
	guint32 data;
	m_device.GetReg(register_address, data);
	/*
	TODO : camera read register
	
	根据地址获取数据 
	*/
	pAckHdr->size = packet->header.size;
	size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->size);

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
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	ArvGvcpPacket* packet = (ArvGvcpPacket*)(pData);
	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;
	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_WRITE_REGISTER_ACK);
	pAckHdr->size = /*htons*/(packet->header.size);
	pAckHdr->id = htons(ntohs(packet->header.id));

	if (_nLastAckId != ntohs(packet->header.id))
	{
		_nLastAckId = ntohs(packet->header.id);
	}
	guint32 payload_size = packet->header.size;
	guint32 register_address = 0;
	guint32 register_value = 0;

	arv_gvcp_packet_get_write_register_cmd_infos(packet, &register_address,&register_value);

	/*
	TODO : camera write register

	*/
	tag_WriteReg_ACK *pAck = (tag_WriteReg_ACK *)(cSendData + sizeof(ArvGvcpHeader));
	pAck->nReserved = 0;
	pAck->nIndex = htons(1);//写成功标识符

	size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->size);

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
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	ArvGvcpPacket* packet = (ArvGvcpPacket*)(pData);
	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;
	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_READ_MEMORY_ACK);
	pAckHdr->size = htons(sizeof(tag_ReadMem_CMD));
	pAckHdr->id = htons(ntohs(packet->header.id));

	if (_nLastAckId != ntohs(packet->header.id))
	{
		_nLastAckId = ntohs(packet->header.id);
	}
	guint32 register_address = 0;
	guint32 block_size = 0;
	tag_ReadMem_CMD* preg =(tag_ReadMem_CMD*)packet->data;
	register_address = preg->nMemAddress;
	block_size = preg->count;

	//arv_gvcp_packet_get_read_memory_cmd_infos(packet, &register_address,&block_size);

	/*
	TODO : camera read register

	*/

	tag_Readmem_ACK *pRegAck = (tag_Readmem_ACK *)(cSendData + sizeof(ArvGvcpHeader));
	pRegAck->nMemAddress = register_address;
	pRegAck->chReadMemData[0];//装载数据

	size_t nLen = sizeof(ArvGvcpHeader) + ntohs(pAckHdr->size);
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
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	ArvGvcpPacket* packet = (ArvGvcpPacket*)(pData);
	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;

	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_WRITE_MEMORY_ACK);
	pAckHdr->size = htons(sizeof(tag_WriteMem_ACK));
	pAckHdr->id = htons(ntohs(packet->header.id));

	if (_nLastAckId != ntohs(packet->header.id))
	{
		_nLastAckId = ntohs(packet->header.id);
	}

	tag_WriteMem_CMD* pWriteData = (tag_WriteMem_CMD*)packet->data;

	guint32 block_address = pWriteData->nMemAddress;//数据的起始地址
	pWriteData->chWriteMemData;//需要写入的数据

	guint32 block_size = ntohs(packet->header.size)-sizeof(guint32);//数据大小  对齐
	//arv_gvcp_packet_get_write_memory_cmd_infos(packet, &block_address,&block_size);

	/*
	TODO : camera write memory

	*/
	tag_WriteMem_ACK *pWData = (tag_WriteMem_ACK *)(cSendData + sizeof(ArvGvcpHeader));
	pWData->nIndex = 536;
	pWData->nReserved = 0;

	size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->size);

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
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	unsigned char *pData = tagdata._byteData->data();
	ArvGvcpPacket* packet = (ArvGvcpPacket*)(pData);
	/*
	fOCUSIP 
	
	*/
	Arv_FocusIP_Msg* pFocusIP = (Arv_FocusIP_Msg*)packet->data;
	


	ArvGvcpHeader* pAckHdr = (ArvGvcpHeader*)cSendData;
	pAckHdr->packet_type = htons(ARV_GVCP_PACKET_TYPE_ACK);
	pAckHdr->command = htons(ARV_GVCP_COMMAND_FOCUSIP_ACK);
	pAckHdr->size = htons(sizeof(DISCOVERY_ACK_MSG));
	pAckHdr->id = htons(ntohs(packet->header.id));

	if (_nLastAckId != ntohs(packet->header.id))
	{
		_nLastAckId = ntohs(packet->header.id);
	}
	guint32 block_address = 0;
	guint32 block_size = 0;
	arv_gvcp_packet_get_write_memory_cmd_infos(packet, &block_address, &block_size);

	/*
	TODO : camera write memory

	*/

	size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->size);

	tagUdpData m_writeData;
	m_writeData.fromPoint = tagdata.fromPoint;
	m_writeData._byteData = boost::shared_ptr<ByteVector>(new ByteVector);
	m_writeData._byteData->resize(nLen, 0);
	unsigned char *pSendData = m_writeData._byteData->data();
	memcpy(pSendData, cSendData, nLen);
	std::string straddress = m_writeData.fromPoint.address().to_string() + ":" + to_string(m_writeData.fromPoint.port());

	m_pUdpServer->writeRaw(m_writeData);
}
