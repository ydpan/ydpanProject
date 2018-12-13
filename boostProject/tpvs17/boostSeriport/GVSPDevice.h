#ifndef _GVSP_DEVICE_H_
#define _GVSP_DEVICE_H_
#include "udpserver.h"
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "DeviceInfo.h"
#include "gvsp.h"
using namespace boost;
typedef unsigned int ArvPixelFormat;
class GVSPDevice
{
public:
	GVSPDevice();
	~GVSPDevice();

	void handMsgCallBack(tagUdpData tagData);

	ArvGvspPacket *arv_gvsp_packet_new(ArvGvspContentType content_type,
		guint16 frame_id, guint32 packet_id, size_t data_size, void *buffer, size_t *buffer_size);
	ArvGvspPacket *arv_gvsp_packet_new_data_leader(guint16 frame_id, guint32 packet_id,
			guint64 timestamp, ArvPixelFormat pixel_format,
			guint32 width, guint32 height,
			guint32 x_offset, guint32 y_offset,
			void *buffer, size_t *buffer_size);
	ArvGvspPacket *arv_gvsp_packet_new_data_trailer(guint16 frame_id, guint32 packet_id, void *buffer, size_t *buffer_size);
	ArvGvspPacket *arv_gvsp_packet_new_data_block(guint16 frame_id, guint32 packet_id, size_t size, void *data, void *buffer, size_t *buffer_size);
private:
	boost::shared_ptr<UDPService> m_udpSocket;
	boost::asio::ip::udp::endpoint _fromPoint;
	boost::asio::ip::udp::endpoint _targetPoint;

	DeviceInfo m_device;
};

#endif
