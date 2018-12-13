#include "GVSPDevice.h"

GVSPDevice::GVSPDevice()
{
	m_udpSocket = boost::make_shared<UDPService>("127.0.0.1", 2300);
	m_udpSocket->setCallbackFunc(boost::bind(&GVSPDevice::handMsgCallBack, this, _1));
	m_udpSocket->startThread();
}


GVSPDevice::~GVSPDevice()
{

}

void GVSPDevice::handMsgCallBack(tagUdpData tagData)
{

}

ArvGvspPacket * GVSPDevice::arv_gvsp_packet_new(ArvGvspContentType content_type, guint16 frame_id, guint32 packet_id, size_t data_size, void *buffer, size_t *buffer_size)
{
	ArvGvspPacket *packet;
	size_t packet_size;

	packet_size = sizeof(ArvGvspPacket) + data_size;
	if (packet_size == 0 || (buffer != NULL && (buffer_size == NULL || packet_size > *buffer_size)))
		return NULL;

	if (buffer_size != NULL)
		*buffer_size = packet_size;

	if (buffer != NULL)
		packet = (ArvGvspPacket *)buffer;
// 	else
// 		packet = malloc(packet_size);

	packet->header.packet_type = 0;
	packet->header.frame_id = htons(frame_id);
	packet->header.packet_infos = htonl((packet_id & ARV_GVSP_PACKET_INFOS_ID_MASK) |
		((content_type << ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_POS) &
			ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_MASK));

	return packet;
}

ArvGvspPacket * GVSPDevice::arv_gvsp_packet_new_data_leader(guint16 frame_id, guint32 packet_id, guint64 timestamp, ArvPixelFormat pixel_format, guint32 width, guint32 height, guint32 x_offset, guint32 y_offset, void *buffer, size_t *buffer_size)
{
	ArvGvspPacket *packet;

	packet = arv_gvsp_packet_new(ARV_GVSP_CONTENT_TYPE_DATA_LEADER,
		frame_id, packet_id, sizeof(ArvGvspDataLeader), buffer, buffer_size);

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
	}

	return packet;
}

ArvGvspPacket *GVSPDevice::arv_gvsp_packet_new_data_trailer(guint16 frame_id, guint32 packet_id,
	void *buffer, size_t *buffer_size)
{
	ArvGvspPacket *packet;

	packet = arv_gvsp_packet_new(ARV_GVSP_CONTENT_TYPE_DATA_TRAILER,
		frame_id, packet_id, sizeof(ArvGvspDataTrailer), buffer, buffer_size);

	if (packet != NULL) {
		ArvGvspDataTrailer *trailer;

		trailer = (ArvGvspDataTrailer *)&packet->data;
		trailer->payload_type = htonl(ARV_GVSP_PAYLOAD_TYPE_IMAGE);
		trailer->data0 = 0;
	}

	return packet;
}

ArvGvspPacket *GVSPDevice::arv_gvsp_packet_new_data_block(guint16 frame_id, guint32 packet_id,
	size_t size, void *data,
	void *buffer, size_t *buffer_size)
{
	ArvGvspPacket *packet;

	packet = arv_gvsp_packet_new(ARV_GVSP_CONTENT_TYPE_DATA_BLOCK,
		frame_id, packet_id, size, buffer, buffer_size);

	if (packet != NULL)
		memcpy(&packet->data, data, size);

	return packet;
}

