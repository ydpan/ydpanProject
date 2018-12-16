#include "GVSPDevice.h"
#define  ARV_GV_FAKE_CAMERA_BUFFER_SIZE 65536
GVSPDevice::GVSPDevice()
{

}


GVSPDevice::~GVSPDevice()
{

}

void GVSPDevice::InitDevice()
{
	m_udpSocket = boost::make_shared<udpSender>(m_iosServer);
// 	m_udpSocket = boost::make_shared<UDPService>("192.168.8.160", 9001);
// 	m_udpSocket->setCallbackFunc(boost::bind(&GVSPDevice::handMsgCallBack, this, _1));
// 	m_udpSocket->startThread();
// 	m_udpSocket->setObjName("GVSP obj");
}
void GVSPDevice::handMsgCallBack(tagUdpData tagData)
{
	int a = 0;
}

ArvGvspPacket * GVSPDevice::arv_gvsp_packet_new(ArvGvspContentType content_type, guint16 frame_id, guint32 packet_id, size_t data_size, 
	tagUdpData &packdata)
{
	size_t buffer_size = ARV_GV_FAKE_CAMERA_BUFFER_SIZE;
	packdata._byteData = boost::make_shared<ByteVector>();
	
	size_t packet_size = sizeof(ArvGvspPacket) + data_size;
	if (packet_size == 0 || packet_size > buffer_size)
		return NULL;
	packdata._byteData->resize(packet_size, 0);
	ArvGvspPacket *packet = (ArvGvspPacket *)packdata._byteData->data();

	packet->header.packet_type = 0;
	packet->header.frame_id = htons(frame_id);
	packet->header.packet_infos = htonl((packet_id & ARV_GVSP_PACKET_INFOS_ID_MASK) |
		((content_type << ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_POS) &
			ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_MASK));

	return (ArvGvspPacket*)packet;
}

ArvGvspPacket * GVSPDevice::gvsp_packet_data_leader(guint16 frame_id, guint32 packet_id, guint64 timestamp, ArvPixelFormat pixel_format, guint32 width,
	guint32 height, guint32 x_offset, guint32 y_offset, tagUdpData &packdata)
{
	ArvGvspPacket *packet = arv_gvsp_packet_new(ARV_GVSP_CONTENT_TYPE_DATA_LEADER,
		frame_id, packet_id, sizeof(ArvGvspDataLeader), packdata);

	if (packet != NULL) {
		ArvGvspDataLeader *leader;

		leader = (ArvGvspDataLeader *)&packet->data;
		leader->flags = 0;
		leader->payload_type = htons(ARV_GVSP_PAYLOAD_TYPE_IMAGE);
		leader->timestamp_high = htonl(((guint64)timestamp >> 32));
		leader->timestamp_low = htonl((guint64)timestamp & 0xffffffff);
		leader->pixel_format = htonl(pixel_format);
		leader->width = htonl(width);
		leader->height = htonl(height);
		leader->x_offset = htonl(x_offset);
		leader->y_offset = htonl(y_offset);
		leader->x_padding = 0;
		leader->y_padding = 0;
	}

	return packet;
}

ArvGvspPacket * GVSPDevice::gvsp_packet_data_trailer(guint16 frame_id, guint32 packet_id, guint32 data0,
	tagUdpData &packdata)
{
	ArvGvspPacket *packet = arv_gvsp_packet_new(ARV_GVSP_CONTENT_TYPE_DATA_TRAILER,
		frame_id, packet_id, sizeof(ArvGvspDataTrailer), packdata);

	if (packet != NULL) {
		ArvGvspDataTrailer *trailer;

		trailer = (ArvGvspDataTrailer *)&packet->data;
		trailer->payload_type = htonl(ARV_GVSP_PAYLOAD_TYPE_IMAGE);
		trailer->data0 = htonl(data0);
	}

	return packet;
}

ArvGvspPacket *GVSPDevice::gvsp_packet_data_block(guint16 frame_id, guint32 packet_id,
	size_t size, void *data, tagUdpData &packdata)
{
	ArvGvspPacket *packet = arv_gvsp_packet_new(ARV_GVSP_CONTENT_TYPE_DATA_BLOCK,
		frame_id, packet_id, size, packdata);

	if (packet != NULL)
	{
		unsigned char* pTaget = (unsigned char*)&packet->data;
		memcpy(pTaget, (unsigned char*)data, size);
	}

	return packet;
}

ArvGvspPacketType GVSPDevice::arv_gvsp_packet_get_packet_type(const ArvGvspPacket *packet)
{
	return (ArvGvspPacketType)ntohs(packet->header.packet_type);
}

ArvGvspContentType GVSPDevice::arv_gvsp_packet_get_content_type(const ArvGvspPacket *packet)
{
	return (ArvGvspContentType)((ntohl(packet->header.packet_infos) & ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_MASK) >>
		ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_POS);
}
guint16 GVSPDevice::arv_gvsp_packet_get_packet_id(const ArvGvspPacket *packet)
{
	return ntohl(packet->header.packet_infos) & ARV_GVSP_PACKET_INFOS_ID_MASK;
}
guint16 GVSPDevice::arv_gvsp_packet_get_frame_id(const ArvGvspPacket *packet)
{
	return ntohs(packet->header.frame_id);
}
BufferPayloadType GVSPDevice::arv_gvsp_packet_get_buffer_payload_type(const ArvGvspPacket *packet)
{
	ArvGvspDataLeader *leader;
	ArvGvspPayloadType gvsp_payload_type;

	leader = (ArvGvspDataLeader *)&packet->data;

	gvsp_payload_type = (ArvGvspPayloadType)ntohs(leader->payload_type);

	switch (gvsp_payload_type) {
	case ARV_GVSP_PAYLOAD_TYPE_IMAGE:
		return ARV_BUFFER_PAYLOAD_TYPE_IMAGE;
	case ARV_GVSP_PAYLOAD_TYPE_RAWDATA:
		return ARV_BUFFER_PAYLOAD_TYPE_RAWDATA;
	case ARV_GVSP_PAYLOAD_TYPE_FILE:
		return ARV_BUFFER_PAYLOAD_TYPE_FILE;
	case ARV_GVSP_PAYLOAD_TYPE_CHUNK_DATA:
		return ARV_BUFFER_PAYLOAD_TYPE_CHUNK_DATA;
	case ARV_GVSP_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA:
		return ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA;
	case ARV_GVSP_PAYLOAD_TYPE_JPEG:
		return ARV_BUFFER_PAYLOAD_TYPE_JPEG;
	case ARV_GVSP_PAYLOAD_TYPE_JPEG2000:
		return ARV_BUFFER_PAYLOAD_TYPE_JPEG2000;
	case ARV_GVSP_PAYLOAD_TYPE_H264:
		return ARV_BUFFER_PAYLOAD_TYPE_H264;
	case ARV_GVSP_PAYLOAD_TYPE_MULTIZONE_IMAGE:
		return ARV_BUFFER_PAYLOAD_TYPE_MULTIZONE_IMAGE;
	}

	return ARV_BUFFER_PAYLOAD_TYPE_UNKNOWN;
}

guint32 GVSPDevice::arv_gvsp_packet_get_x_offset(const ArvGvspPacket *packet)
{
	ArvGvspDataLeader *leader;

	leader = (ArvGvspDataLeader *)&packet->data;
	return ntohl(leader->x_offset);
}

guint32 GVSPDevice::arv_gvsp_packet_get_y_offset(const ArvGvspPacket *packet)
{
	ArvGvspDataLeader *leader;

	leader = (ArvGvspDataLeader *)&packet->data;
	return ntohl(leader->y_offset);
}

guint32 GVSPDevice::arv_gvsp_packet_get_width(const ArvGvspPacket *packet)
{
	ArvGvspDataLeader *leader;

	leader = (ArvGvspDataLeader *)&packet->data;
	return ntohl(leader->width);
}

guint32 GVSPDevice::arv_gvsp_packet_get_height(const ArvGvspPacket *packet)
{
	ArvGvspDataLeader *leader;

	leader = (ArvGvspDataLeader *)&packet->data;
	return ntohl(leader->height);
}
ArvPixelFormat GVSPDevice::arv_gvsp_packet_get_pixel_format(const ArvGvspPacket *packet)
{
	ArvGvspDataLeader *leader;

	leader = (ArvGvspDataLeader *)&packet->data;
	return (ArvPixelFormat)ntohl(leader->pixel_format);
}
guint64 GVSPDevice::arv_gvsp_packet_get_timestamp(const ArvGvspPacket *packet, guint64 timestamp_tick_frequency)
{
	ArvGvspDataLeader *leader;
	guint64 timestamp_s;
	guint64 timestamp_ns;
	guint64 timestamp;

	if (timestamp_tick_frequency < 1)
		return 0;

	leader = (ArvGvspDataLeader *)&packet->data;

	timestamp = ((guint64)ntohl(leader->timestamp_high) << 32) | ntohl(leader->timestamp_low);

	timestamp_s = timestamp / timestamp_tick_frequency;
	timestamp_ns = ((timestamp % timestamp_tick_frequency) * 1000000000) / timestamp_tick_frequency;

	timestamp_ns += timestamp_s * 1000000000;

	return timestamp_ns;
}
size_t GVSPDevice::arv_gvsp_packet_get_data_size(size_t packet_size)
{
	return packet_size - sizeof(ArvGvspHeader);
}

void GVSPDevice::sendData(tagUdpData &packData)
{
	//packData.fromPoint = _targetPoint;
	std::string straddress = packData.fromPoint.address().to_string() + ":" + to_string(packData.fromPoint.port());
	m_udpSocket->writeRaw(packData);
}

