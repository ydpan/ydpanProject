#include <WinSock2.h>
#ifndef _GVCP_H_
#define _GVCP_H_

typedef unsigned short guint16;
typedef unsigned int   guint32;

/**
 * GVCP_PORT:
 *
 * Standard device listening port for GVCP packets
 */
#define ARV_GVCP_PORT	3956
//device info
#define ARV_GVBS_VERSION_OFFSET				0x00000000
#define ARV_GVBS_VERSION_MINOR_MASK			0x0000ffff
#define ARV_GVBS_VERSION_MINOR_POS			0
#define ARV_GVBS_VERSION_MAJOR_MASK			0xffff0000
#define ARV_GVBS_VERSION_MAJOR_POS			16

#define ARV_GVBS_DEVICE_MODE_OFFSET			0x00000004
#define ARV_GVBS_DEVICE_MODE_BIG_ENDIAN			1 << 31
#define ARV_GVBS_DEVICE_MODE_CHARACTER_SET_MASK		0x0000ffff
#define ARV_GVBS_DEVICE_MODE_CHARACTER_SET_POS		0

#define ARV_GVBS_DEVICE_MAC_ADDRESS_HIGH_OFFSET		0x00000008
#define ARV_GVBS_DEVICE_MAC_ADDRESS_LOW_OFFSET		0x0000000c

#define ARV_GVBS_SUPPORTED_IP_CONFIGURATION_OFFSET	0x00000010
#define ARV_GVBS_CURRENT_IP_CONFIGURATION_OFFSET	0x00000014
#define ARV_GVBS_IP_CONFIGURATION_PERSISTENT		1 << 0
#define ARV_GVBS_IP_CONFIGURATION_DHCP			1 << 1
#define ARV_GVBS_IP_CONFIGURATION_LLA			1 << 2

#define ARV_GVBS_CURRENT_IP_ADDRESS_OFFSET		0x00000024
#define ARV_GVBS_CURRENT_SUBNET_MASK_OFFSET		0x00000034
#define ARV_GVBS_CURRENT_GATEWAY_OFFSET			0x00000044

#define ARV_GVBS_MANUFACTURER_NAME_OFFSET		0x00000048
#define ARV_GVBS_MANUFACTURER_NAME_SIZE			32

#define ARV_GVBS_MODEL_NAME_OFFSET			0x00000068
#define ARV_GVBS_MODEL_NAME_SIZE			32

#define ARV_GVBS_DEVICE_VERSION_OFFSET			0x00000088
#define ARV_GVBS_DEVICE_VERSION_SIZE			32

#define ARV_GVBS_MANUFACTURER_INFORMATIONS_OFFSET	0x000000a8
#define ARV_GVBS_MANUFACTURER_INFORMATIONS_SIZE		48

#define ARV_GVBS_SERIAL_NUMBER_OFFSET			0x000000d8
#define ARV_GVBS_SERIAL_NUMBER_SIZE			16

#define ARV_GVBS_USER_DEFINED_NAME_OFFSET		0x000000e8
#define ARV_GVBS_USER_DEFINED_NAME_SIZE			16
//
#define ARV_GVBS_DISCOVERY_DATA_SIZE			0xf8

#define ARV_GVBS_XML_URL_0_OFFSET			0x00000200
#define ARV_GVBS_XML_URL_1_OFFSET			0x00000400
#define ARV_GVBS_XML_URL_SIZE				512

#define ARV_GVBS_N_NETWORK_INTERFACES_OFFSET		0x00000600

#define	ARV_GVBS_PERSISTENT_IP_ADDRESS_0_OFFSET		0x0000064c
#define	ARV_GVBS_PERSISTENT_SUBNET_MASK_0_OFFSET	0x0000065c
#define	ARV_GVBS_PERSISTENT_GATEWAY_0_OFFSET		0x0000066c

#define ARV_GVBS_N_MESSAGE_CHANNELS_OFFSET		0x00000900
#define ARV_GVBS_N_STREAM_CHANNELS_OFFSET		0x00000904

#define ARV_GVBS_GVCP_CAPABILITY_OFFSET			0x00000934
#define ARV_GVBS_GVCP_CAPABILITY_CONCATENATION			1 << 0
#define ARV_GVBS_GVCP_CAPABILITY_WRITE_MEMORY			1 << 1
#define ARV_GVBS_GVCP_CAPABILITY_PACKET_RESEND			1 << 2
#define ARV_GVBS_GVCP_CAPABILITY_EVENT				1 << 3
#define ARV_GVBS_GVCP_CAPABILITY_EVENT_DATA			1 << 4
#define ARV_GVBS_GVCP_CAPABILITY_PENDING_ACK			1 << 5
#define ARV_GVBS_GVCP_CAPABILITY_ACTION				1 << 6
#define ARV_GVBS_GVCP_CAPABILITY_PRIMARY_APPLICATION_SWITCHOVER 1 << 21
#define ARV_GVBS_GVCP_CAPABILITY_EXTENDED_STATUS_CODES		1 << 22
#define ARV_GVBS_GVCP_CAPABILITY_DISCOVERY_ACK_DELAY_WRITABLE	1 << 23
#define ARV_GVBS_GVCP_CAPABILITY_DISCOVERY_ACK_DELAY		1 << 24
#define ARV_GVBS_GVCP_CAPABILITY_TEST_DATA			1 << 25
#define ARV_GVBS_GVCP_CAPABILITY_MANIFEST_TABLE			1 << 26
#define ARV_GVBS_GVCP_CAPABILITY_CCP_APPLICATION_SOCKET 	1 << 27
#define ARV_GVBS_GVCP_CAPABILITY_LINK_SPEED			1 << 28
#define ARV_GVBS_GVCP_CAPABILITY_HEARTBEAT_DISABLE		1 << 29
#define ARV_GVBS_GVCP_CAPABILITY_SERIAL_NUMBER			1 << 30
#define ARV_GVBS_GVCP_CAPABILITY_NAME_REGISTER			1 << 31

#define ARV_GVBS_HEARTBEAT_TIMEOUT_OFFSET		0x00000938
#define ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_HIGH_OFFSET	0x0000093c
#define ARV_GVBS_TIMESTAMP_TICK_FREQUENCY_LOW_OFFSET	0x00000940
#define ARV_GVBS_TIMESTAMP_CONTROL_OFFSET		0x00000944
#define ARV_GVBS_TIMESTAMP_LATCHED_VALUE_HIGH_OFFSET	0x00000948
#define ARV_GVBS_TIMESTAMP_LATCHED_VALUE_LOW_OFFSET	0x0000094c

#define ARV_GVBS_CONTROL_CHANNEL_PRIVILEGE_OFFSET	0x00000a00//control channel privilege
#define ARV_GVBS_CONTROL_CHANNEL_PRIVILEGE_CONTROL	1 << 1
#define ARV_GVBS_CONTROL_CHANNEL_PRIVILEGE_EXCLUSIVE	1 << 0

#define ARV_GVBS_STREAM_CHANNEL_0_PORT_OFFSET		0x00000d00

#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_OFFSET		0x00000d04
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_MASK		0x0000ffff
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_POS		0
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_BIG_ENDIAN		1 << 29
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_DO_NOT_FRAGMENT	1 << 30
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_FIRE_TEST		1 << 31

#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_DELAY_OFFSET		0x00000d08

#define ARV_GVBS_STREAM_CHANNEL_0_IP_ADDRESS_OFFSET		0x00000d18

#define ARV_GVBS_DEVICE_LINK_SPEED_0_OFFSET			0x0000b000

#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_MIN_OFFSET		0x0000c000
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_SIZE_MAX_OFFSET		0x0000c004
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_DELAY_MIN_OFFSET		0x0000c008
#define ARV_GVBS_STREAM_CHANNEL_0_PACKET_DELAY_MAX_OFFSET		0x0000c00c
#define ARV_GVBS_STREAM_CHANNEL_0_FRAME_TRANSMISSION_DELAY_OFFSET	0x0000c010
#define ARV_GVBS_STREAM_CHANNEL_0_FRAME_TRANSMISSION_DELAY_MIN_OFFSET	0x0000c014
#define ARV_GVBS_STREAM_CHANNEL_0_FRAME_TRANSMISSION_DELAY_MAX_OFFSET	0x0000c018
#define ARV_GVBS_STREAM_CHANNEL_0_BANDWITDH_RESERVE_OFFSET		0x0000c01c
#define ARV_GVBS_STREAM_CHANNEL_0_BANDWITDH_RESERVE_MIN_OFFSET		0x0000c020
#define ARV_GVBS_STREAM_CHANNEL_0_BANDWITDH_RESERVE_MAX_OFFSET		0x0000c024
#define ARV_GVBS_STREAM_CHANNEL_0_BANDWITDH_RESERVE_ACCUMULATION_OFFSET	0x0000c028
#define ARV_GVBS_STREAM_CHANNEL_0_BANDWITDH_RESERVE_ACCUMULATION_MIN_OFFSET	0x0000c02c
#define ARV_GVBS_STREAM_CHANNEL_0_BANDWITDH_RESERVE_ACCUMULATION_MAX_OFFSET	0x0000c030
#define ARV_GVBS_STREAM_CHANNEL_0_THROUGHPUT_MAX_OFFSET			0x0000c100
#define ARV_GVBS_STREAM_CHANNEL_0_CURRENT_THOURGHPUT_OFFSET		0x0000c104
#define ARV_GVBS_STREAM_CHANNEL_0_ASSIGNED_BANDWIDTH_OFFSET		0x0000c108
#define ARV_GVBS_STREAM_CHANNEL_0_FRAME_JITTER_MAX_OFFSET		0x0000c10c

#define ARV_GVCP_DATA_SIZE_MAX				512

/**
 * ArvGvcpPacketType:
 * @ARV_GVCP_PACKET_TYPE_ACK: acknowledge packet
 * @ARV_GVCP_PACKET_TYPE_RESEND: resend request packet
 * @ARV_GVCP_PACKET_TYPE_CMD: command packet
 * @ARV_GVCP_PACKET_TYPE_ERROR: error packet
 */

typedef enum {
	ARV_GVCP_PACKET_TYPE_ACK = 0x0000,
	ARV_GVCP_PACKET_TYPE_RESEND = 0x4200,
	ARV_GVCP_PACKET_TYPE_CMD = 0x4201,
	ARV_GVCP_PACKET_TYPE_ERROR = 0x8006
} ArvGvcpPacketType,GvcpPacketType;

/**
 * ArvGvcpCommand:
 * @ARV_GVCP_COMMAND_DISCOVERY_CMD: discovery command
 * @ARV_GVCP_COMMAND_DISCOVERY_ACK: discovery acknowledge
 * @ARV_GVCP_COMMAND_BYE_CMD: goodbye command, for connection termination
 * @ARV_GVCP_COMMAND_BYE_ACK: goodbye acknowledge
 * @ARV_GVCP_COMMAND_PACKET_RESEND_CMD: packet resend request
 * @ARV_GVCP_COMMAND_PACKET_RESEND_ACK: packet resend acknowledge (not used ?)
 * @ARV_GVCP_COMMAND_READ_REGISTER_CMD: read register command
 * @ARV_GVCP_COMMAND_READ_REGISTER_ACK: read register acknowledge
 * @ARV_GVCP_COMMAND_WRITE_REGISTER_CMD: write register command
 * @ARV_GVCP_COMMAND_WRITE_REGISTER_ACK: write register acknowledge
 * @ARV_GVCP_COMMAND_READ_MEMORY_CMD: read memory command
 * @ARV_GVCP_COMMAND_READ_MEMORY_ACK: read memory acknowledge
 * @ARV_GVCP_COMMAND_WRITE_MEMORY_CMD: write memory command
 * @ARV_GVCP_COMMAND_WRITE_MEMORY_ACK: write memory acknowledge
 * @ARV_GVCP_COMMAND_PENDING_ACK: pending command acknowledge
 */

typedef enum {
	ARV_GVCP_COMMAND_DISCOVERY_CMD = 0x0002,
	ARV_GVCP_COMMAND_DISCOVERY_ACK = 0x0003,
	ARV_GVCP_COMMAND_FOCUSIP_CMD = 0x0004,
	ARV_GVCP_COMMAND_FOCUSIP_ACK = 0x0005,
	ARV_GVCP_COMMAND_PACKET_RESEND_CMD = 0x0040,
	ARV_GVCP_COMMAND_PACKET_RESEND_ACK = 0x0041,
	ARV_GVCP_COMMAND_READ_REGISTER_CMD = 0x0080,
	ARV_GVCP_COMMAND_READ_REGISTER_ACK = 0x0081,
	ARV_GVCP_COMMAND_WRITE_REGISTER_CMD = 0x0082,
	ARV_GVCP_COMMAND_WRITE_REGISTER_ACK = 0x0083,
	ARV_GVCP_COMMAND_READ_MEMORY_CMD = 0x0084,
	ARV_GVCP_COMMAND_READ_MEMORY_ACK = 0x0085,
	ARV_GVCP_COMMAND_WRITE_MEMORY_CMD = 0x0086,
	ARV_GVCP_COMMAND_WRITE_MEMORY_ACK = 0x0087,
	ARV_GVCP_COMMAND_PENDING_ACK = 0x0089
} ArvGvcpCommand, GvcpCommand;

#define ARV_GVCP_MAX_MSG_LEN 548
#define ARV_GVCP_MAX_PAYLOAD_LEN         540

/**
 * ArvGvcpHeader:
 * @packet_type: a #ArvGvcpPacketType identifier
 * @command: a #ArvGvcpCommand identifier
 * @size: data size
 * @id: packet identifier
 *
 * GVCP packet header structure.
 */

typedef struct tag_ArvGvcpHeader{
	unsigned short packet_type;//constains 0x42 flag=1
	unsigned short command;
	unsigned short nlength;//结构体后负载数据长度
	unsigned short nRequesd_Id;
} ArvGvcpHeader;//for recved


/**
 * ArvGvcpPacket:
 * @header: packet header
 * @data: variable size byte array
 *
 * GVCP packet structure.
 */

typedef struct {
	ArvGvcpHeader header;
	unsigned char bodydata[];
} ArvGvcpPacket;

typedef struct tag_Discovery_Ack
{
	unsigned short nMajorVer;
	unsigned short nMinorVer;
	unsigned int   nDeviceMode;
	unsigned short nRes1;
	unsigned short nMacAddrHigh;
	unsigned int   nMacAddrLow;
	unsigned int   nIpCfgOption;
	unsigned int   nIpCfgCurrent;       //IP configuration:bit31-static bit30-dhcp bit29-lla
	unsigned int   nRes2[3];
	unsigned int   nCurrentIp;
	unsigned int   nRes3[3];
	unsigned int   nCurrentSubNetMask;
	unsigned int   nRes4[3];
	unsigned int   nDefultGateWay;
	unsigned char  chManufacturerName[32];
	unsigned char  chModelName[32];
	unsigned char  chDeviceVersion[32];
	unsigned char  chManufacturerSpecificInfo[48];
	unsigned char  chSerialNumber[16];
	unsigned char  chUserDefinedName[16];
} DISCOVERY_ACK_MSG;

/*
forceip_cmd
需结合数据头的flag使用
TODO 参考65页说明
*/
typedef struct tag_FocusIP_CMD//强制配置IP 数据结构  根据msk类型配置IP
{
	unsigned short      Reserved0;
	unsigned short      MacAddrHigh;//mask 地址高字节
	unsigned int        MacAddrLow;
	unsigned int        Reserved1[3];
	unsigned int        StaticIp;      // 如果此字段为0，设备必须在全部的网络接口上重启IP配置，而不需要回复确认消息;
	unsigned int        Reserved2[3];
	unsigned int        StaticSubNetMask;//掩码
	unsigned int        Reserved3[3];
	unsigned int        StaticDefaultGateWay;//默认网关
} Arv_FocusIP_Msg;
/*
forceip_ack
status = 0;
acknowledge=Forceip_ack
lengyth=0;
ack_id

*/

#define MV_MAX_WRITEREG_INDEX      67
typedef struct tag_WriteReg_ACK
{
	unsigned short      nReserved;   // 置0
	unsigned short      nIndex;      // 表示第n（0-66）个写入出错。如果都正确，填67
} WRITEREG_ACK_MSG;

typedef struct tag_WriteReg_CMD
{
	unsigned int       nRegAddress;   // 寄存器地址
	unsigned int       nRegData;      // 地址对应的数据
} WRITEREG_CMD_MSG;

typedef struct tag_ReadMem_CMD
{
	unsigned int        nMemAddress;     // 4bytes 对齐的地址
	unsigned short      nReserved{ 0 };      // 置0
	unsigned short      count;        // 读取的byte数量。（4的倍数）
} READMEM_CMD_MSG;

typedef struct tag_Readmem_ACK
{
	unsigned long       nMemAddress;    // 4bytes 对齐的地址
	unsigned char       cReadMemData[ARV_GVCP_MAX_PAYLOAD_LEN]; // 读取的内存数据
} READMEM_ACK_MSG;

typedef struct tag_WriteMem_CMD
{
	unsigned long       nMemAddress;    // 4bytes 对齐的地址
	unsigned char       cWriteMemData[ARV_GVCP_MAX_PAYLOAD_LEN]; // 待写的内存数据
} WRITEMEM_CMD_MSG;

typedef struct tag_WriteMem_ACK
{
	unsigned short      nReserved;   // 置0
	unsigned short      nIndex;      // 成功，填总写入数据量；失败，表示第n（0-535）个写入出错。
} WRITEMEM_ACK_MSG;

void 			arv_gvcp_packet_free(ArvGvcpPacket *packet);
ArvGvcpPacket * arv_gvcp_packet_new_read_memory_cmd(guint32 address, guint32 size, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_read_memory_ack(guint32 address, guint32 size, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_write_memory_cmd(guint32 address, guint32 size, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_write_memory_ack(guint32 address, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_read_register_cmd(guint32 address, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_read_register_ack(guint32 value, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_write_register_cmd(guint32 address, guint32 value, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_write_register_ack(guint32 data_index, guint16 packet_id, size_t *packet_size);
ArvGvcpPacket * arv_gvcp_packet_new_discovery_cmd(size_t *size);
ArvGvcpPacket * arv_gvcp_packet_new_discovery_ack(size_t *packet_size);
// ArvGvcpPacket * 	arv_gvcp_packet_new_packet_resend_cmd(guint32 frame_id,
// 	guint16 first_block, guint32 last_block,
// 	guint16 packet_id, size_t *packet_size);
// char * 			arv_gvcp_packet_to_string(const ArvGvcpPacket *packet);
/*void 			arv_gvcp_packet_debug(const ArvGvcpPacket *packet, ArvDebugLevel level);*/

/**
 * arv_gvcp_packet_get_packet_type:
 * @packet: a #ArvGvcpPacket
 *
 * Return value: The #ArvGvcpPacketType code of @packet.
 */

//返回数据包类型
static inline ArvGvcpPacketType arv_gvcp_packet_get_packet_type(ArvGvcpPacket *packet)
{
	if (packet == NULL)
		return ARV_GVCP_PACKET_TYPE_ERROR;

	return (ArvGvcpPacketType)ntohs(packet->header.packet_type);
}

/**
 * arv_gvcp_packet_get_command:
 * @packet: a #ArvGvcpPacket
 *
 * Return value: The #ArvGvcpCommand code of @packet.
 */

static inline ArvGvcpCommand arv_gvcp_packet_get_command(ArvGvcpPacket *packet)
{
	if (packet == NULL)
		return (ArvGvcpCommand)0;

	return (ArvGvcpCommand)ntohs(packet->header.command);
}

static inline void arv_gvcp_packet_set_packet_id(ArvGvcpPacket *packet, unsigned short id)
{
	if (packet != NULL)
		packet->header.nRequesd_Id = htons(id);
}

static inline unsigned short arv_gvcp_packet_get_packet_id(ArvGvcpPacket *packet)
{
	if (packet == NULL)
		return 0;

	return ntohs(packet->header.nRequesd_Id);
}

static inline void arv_gvcp_packet_get_read_memory_cmd_infos(const ArvGvcpPacket *packet, guint32 *address, guint32 *size)
{
	if (packet == NULL) {
		if (address != NULL)
			*address = 0;
		if (size != NULL)
			*size = 0;
		return;
	}
	if (address != NULL)
		*address = ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket))));//数据地址
	if (size != NULL)
		*size = (ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket) + sizeof(guint32))))) &
		0xffff;//后半截为数据
}

static inline size_t arv_gvcp_packet_get_read_memory_ack_size(guint32 data_size)
{
	return sizeof(ArvGvcpHeader) + sizeof(guint32) + data_size;
}

static inline void * arv_gvcp_packet_get_read_memory_ack_data(const ArvGvcpPacket *packet)
{
	return (char *)packet + sizeof(ArvGvcpHeader) + sizeof(guint32);
}

static inline void arv_gvcp_packet_get_write_memory_cmd_infos(const ArvGvcpPacket *packet, guint32 *address, guint32 *size)
{
	if (packet == NULL) {
		if (address != NULL)
			*address = 0;
		if (size != NULL)
			*size = 0;
		return;
	}
	if (address != NULL)
		*address = ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket))));
	if (size != NULL)
		*size = ntohs(packet->header.nlength) - sizeof(guint32);
}

static inline void * arv_gvcp_packet_get_write_memory_cmd_data(const ArvGvcpPacket *packet)
{
	return (char *)packet + sizeof(ArvGvcpPacket) + sizeof(guint32);
}

static inline size_t arv_gvcp_packet_get_write_memory_ack_size(void)
{
	return sizeof(ArvGvcpPacket) + sizeof(guint32);
}

static inline void arv_gvcp_packet_get_read_register_cmd_infos(const ArvGvcpPacket *packet, guint32 *address)
{
	if (packet == NULL) {
		if (address != NULL)
			*address = 0;
		return;
	}
	if (address != NULL)
		*address = ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket))));
}

static inline guint32 arv_gvcp_packet_get_read_register_ack_value(const ArvGvcpPacket *packet)
{
	if (packet == NULL)
		return 0;
	return ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket))));
}

static inline void arv_gvcp_packet_get_write_register_cmd_infos(const ArvGvcpPacket *packet, guint32 *address, guint32 *value)
{
	if (packet == NULL) {
		if (address != NULL)
			*address = 0;
		if (value != NULL)
			*value = 0;
		return;
	}
	if (address != NULL)
		*address = ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket))));//解析获取数据地址
	if (value != NULL)
		*value = ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket) + sizeof(guint32))));//解析获取数据
}

static inline guint16 arv_gvcp_next_packet_id(guint16 packet_id)
{
	/* packet_id == 0 is an error value */
	if (packet_id == 0xffff)
		return 1;
	return packet_id + 1;
}

/**
 * arv_gvcp_packet_get_pending_ack_timeout:
 * @packet: a #ArvGvcpPacket
 *
 * Returns: The pending acknowledge timeout stored in @packet, in ms.
 *
 * Since: 0.6.0
 */

static inline guint32 arv_gvcp_packet_get_pending_ack_timeout(const ArvGvcpPacket *packet)
{
	return packet != NULL ? ntohl(*((guint32 *)((char *)packet + sizeof(ArvGvcpPacket)))) : 0;
}


#endif // 

