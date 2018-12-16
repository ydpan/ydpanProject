#ifndef _GVSP_H
#define _GVSP_H

#define ARV_GVSP_PACKET_INFOS_ID_MASK		0x00ffffff
#define ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_MASK	0xff000000
#define ARV_GVSP_PACKET_INFOS_CONTENT_TYPE_POS	24

#define ARV_GVSP_PACKET_PROTOCOL_OVERHEAD	(20 + 8 + 8)		/* IP + UDP + GVSP headers */

/**
 * ArvGvspPacketType:
 * @ARV_GVSP_PACKET_TYPE_OK: valid packet
 * @ARV_GVSP_PACKET_TYPE_RESEND: resent packet (BlackFly PointGrey camera support)
 * @ARV_GVSP_PACKET_TYPE_ERROR: error packet, indicating invalid resend request
 */

typedef enum {
	ARV_GVSP_PACKET_TYPE_OK =		0x0000,
	ARV_GVSP_PACKET_TYPE_RESEND =		0x0100,
	ARV_GVSP_PACKET_TYPE_ERROR =		0x800c
} ArvGvspPacketType;

/**
 * ArvGvspContentType:
 * @ARV_GVSP_CONTENT_TYPE_DATA_LEADER: leader packet
 * @ARV_GVSP_CONTENT_TYPE_DATA_TRAILER: trailer packet
 * @ARV_GVSP_CONTENT_TYPE_DATA_BLOCK: data packet
 */

typedef enum {
	ARV_GVSP_CONTENT_TYPE_DATA_LEADER = 	0x01,
	ARV_GVSP_CONTENT_TYPE_DATA_TRAILER = 	0x02,
	ARV_GVSP_CONTENT_TYPE_DATA_BLOCK =	0x03
} ArvGvspContentType;

/**
 * ArvGvspPayloadType:
 * @ARV_GVSP_PAYLOAD_TYPE_IMAGE: image data
 * @ARV_GVSP_PAYLOAD_TYPE_RAWDATA: raw data
 * @ARV_GVSP_PAYLOAD_TYPE_FILE: file
 * @ARV_GVSP_PAYLOAD_TYPE_CHUNK_DATA: chunk data
 * @ARV_GVSP_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA: extended chunk data
 * @ARV_GVSP_PAYLOAD_TYPE_JPEG: JPEG data
 * @ARV_GVSP_PAYLOAD_TYPE_JPEG2000: JPEG2000 data
 * @ARV_GVSP_PAYLOAD_TYPE_H264: h264 data
 * @ARV_GVSP_PAYLOAD_TYPE_MULTIZONE_IMAGE: multizone image
*/

typedef enum {
	ARV_GVSP_PAYLOAD_TYPE_IMAGE =			0x0001,
	ARV_GVSP_PAYLOAD_TYPE_RAWDATA = 		0x0002,
	ARV_GVSP_PAYLOAD_TYPE_FILE = 			0x0003,
	ARV_GVSP_PAYLOAD_TYPE_CHUNK_DATA = 		0x0004,
	ARV_GVSP_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA = 	0x0005, /* Deprecated */
	ARV_GVSP_PAYLOAD_TYPE_JPEG = 			0x0006,
	ARV_GVSP_PAYLOAD_TYPE_JPEG2000 = 		0x0007,
	ARV_GVSP_PAYLOAD_TYPE_H264 = 			0x0008,
	ARV_GVSP_PAYLOAD_TYPE_MULTIZONE_IMAGE = 	0x0009
} ArvGvspPayloadType;

/**
 * ArvGvspHeader:
 * @packet_type: a #ArvGvspPacketType identifier
 * @frame_id: frame identifier
 * @packet_infos: #ArvGvspContentType and packet identifier in a 32 bit value
 *
 * GVSP packet header structure.
 */

typedef struct {
	unsigned short packet_type;
	unsigned short frame_id;
	unsigned int   packet_infos;
} ArvGvspHeader;

/**
 * ArvGvspDataLeader:
 * @flags: generic flags
 * @payload_type: ID of the payload type
 * @timestamp_high: most significant bits of frame timestamp
 * @timestamp_low: least significant bits of frame timestamp_low
 * @pixel_format: a #ArvPixelFormat identifier
 * @width: frame width, in pixels
 * @height: frame height, in pixels
 * @x_offset: frame x offset, in pixels
 * @y_offset: frame y offset, in pixels
 *
 * GVSP data leader packet data area.
 */

typedef struct {
	unsigned short	flags;
	unsigned short	payload_type;
	unsigned int	timestamp_high;
	unsigned int	timestamp_low;
	unsigned int	pixel_format;
	unsigned int	width;
	unsigned int	height;
	unsigned int	x_offset;
	unsigned int	y_offset;
	unsigned int    x_padding;
	unsigned int    y_padding;
} ArvGvspDataLeader,GVSP_Image_Data_Leader;

/**
 * ArvGvspDataTrailer:
 * @payload_type: ID of the payload type
 * @data0: unused
 *
 * GVSP data trailer packet data area.
 */
typedef struct {
	unsigned int payload_type;//数据包类型（IMAGE）
	unsigned int data0;//可用于作为图像的高度
} ArvGvspDataTrailer,GVSP_Image_Data_Trailer;

/**
 * ArvGvspPacket:
 * @header: common GVSP packet header
 * @data: data byte array
 *
 * GVSP packet structure.
 */
typedef struct {
	ArvGvspHeader header;
	unsigned char data[];
} ArvGvspPacket;

typedef unsigned int guint32;
typedef unsigned short guint16;
typedef unsigned long long guint64;

#endif
