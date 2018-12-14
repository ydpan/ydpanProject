#ifndef _GVSP_DEVICE_H_
#define _GVSP_DEVICE_H_
#include "udpserver.h"
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "DeviceInfo.h"
#include "gvsp.h"
#include "ImageBuffer.h"
using namespace boost;
typedef unsigned int ArvPixelFormat;
class GVSPDevice
{
public:
	GVSPDevice();
	~GVSPDevice();

	void handMsgCallBack(tagUdpData tagData);

	ArvGvspPacket *arv_gvsp_packet_new(
		ArvGvspContentType content_type,
		guint16 frame_id, 
		guint32 packet_id, 
		size_t data_size, 
		tagUdpData &packdata);
	//生成新建的数据包

	ArvGvspPacket *gvsp_packet_data_leader(
		guint16 frame_id, 
		guint32 packet_id,
		guint64 timestamp, 
		ArvPixelFormat pixel_format,
		guint32 width, 
		guint32 height,
		guint32 x_offset, 
		guint32 y_offset,
		tagUdpData &packdata);//生成leader包
	//
	ArvGvspPacket *gvsp_packet_data_trailer(
		guint16 frame_id, 
		guint32 packet_id, 
		tagUdpData &packdata);//生成结束数据包

	ArvGvspPacket *gvsp_packet_data_block(
		guint16 frame_id, 
		guint32 packet_id, 
		size_t size, 
		void *data, 
		tagUdpData &packdata);//生成block数据块数据包

	ArvGvspPacketType arv_gvsp_packet_get_packet_type(const ArvGvspPacket *packet);
	ArvGvspContentType arv_gvsp_packet_get_content_type(const ArvGvspPacket *packet);
	guint16 arv_gvsp_packet_get_packet_id(const ArvGvspPacket *packet);
	guint16 arv_gvsp_packet_get_frame_id(const ArvGvspPacket *packet);
	BufferPayloadType arv_gvsp_packet_get_buffer_payload_type(const ArvGvspPacket *packet);
	guint32 arv_gvsp_packet_get_x_offset(const ArvGvspPacket *packet);
	guint32 arv_gvsp_packet_get_y_offset(const ArvGvspPacket *packet);
	guint32 arv_gvsp_packet_get_width(const ArvGvspPacket *packet);
	guint32 arv_gvsp_packet_get_height(const ArvGvspPacket *packet);
	ArvPixelFormat arv_gvsp_packet_get_pixel_format(const ArvGvspPacket *packet);
	guint64 arv_gvsp_packet_get_timestamp(const ArvGvspPacket *packet, guint64 timestamp_tick_frequency);
	size_t arv_gvsp_packet_get_data_size(size_t packet_size);

	void sendData(tagUdpData &packData);
private:
	boost::shared_ptr<UDPService> m_udpSocket;
	boost::asio::ip::udp::endpoint _fromPoint;
	boost::asio::ip::udp::endpoint _targetPoint;
	DeviceInfo m_device;
	
};

#endif
