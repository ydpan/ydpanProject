#ifndef _BUFFER_H_
#define _BUFFER_H_
/**
 * ArvBufferStatus:
 * @ARV_BUFFER_STATUS_UNKNOWN: unknown status
 * @ARV_BUFFER_STATUS_SUCCESS: the buffer contains a valid image
 * @ARV_BUFFER_STATUS_CLEARED: the buffer is cleared
 * @ARV_BUFFER_STATUS_TIMEOUT: timeout was reached before all packets are received
 * @ARV_BUFFER_STATUS_MISSING_PACKETS: stream has missing packets
 * @ARV_BUFFER_STATUS_WRONG_PACKET_ID: stream has packet with wrong id
 * @ARV_BUFFER_STATUS_SIZE_MISMATCH: the received image didn't fit in the buffer data space
 * @ARV_BUFFER_STATUS_FILLING: the image is currently being filled
 * @ARV_BUFFER_STATUS_ABORTED: the filling was aborted before completion
 */

typedef enum {
	ARV_BUFFER_STATUS_UNKNOWN = -1,
	ARV_BUFFER_STATUS_SUCCESS = 0,
	ARV_BUFFER_STATUS_CLEARED,
	ARV_BUFFER_STATUS_TIMEOUT,
	ARV_BUFFER_STATUS_MISSING_PACKETS,
	ARV_BUFFER_STATUS_WRONG_PACKET_ID,
	ARV_BUFFER_STATUS_SIZE_MISMATCH,
	ARV_BUFFER_STATUS_FILLING,
	ARV_BUFFER_STATUS_ABORTED
} BufferStatus;
/**
 * ArvBufferPayloadType:
 * @ARV_BUFFER_PAYLOAD_TYPE_UNKNOWN: unknown payload type
 * @ARV_BUFFER_PAYLOAD_TYPE_IMAGE: image data
 * @ARV_BUFFER_PAYLOAD_TYPE_RAWDATA: raw data
 * @ARV_BUFFER_PAYLOAD_TYPE_FILE: file
 * @ARV_BUFFER_PAYLOAD_TYPE_CHUNK_DATA: chunk data
 * @ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA: extended chunk data
 * @ARV_BUFFER_PAYLOAD_TYPE_JPEG: JPEG data
 * @ARV_BUFFER_PAYLOAD_TYPE_JPEG2000: JPEG2000 data
 * @ARV_BUFFER_PAYLOAD_TYPE_H264: h264 data
 * @ARV_BUFFER_PAYLOAD_TYPE_MULTIZONE_IMAGE: multizone image
*/

typedef enum {
	ARV_BUFFER_PAYLOAD_TYPE_UNKNOWN = -1,
	ARV_BUFFER_PAYLOAD_TYPE_IMAGE = 0x0001,
	ARV_BUFFER_PAYLOAD_TYPE_RAWDATA = 0x0002,
	ARV_BUFFER_PAYLOAD_TYPE_FILE = 0x0003,
	ARV_BUFFER_PAYLOAD_TYPE_CHUNK_DATA = 0x0004,
	ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA = 0x0005, /* Deprecated */
	ARV_BUFFER_PAYLOAD_TYPE_JPEG = 0x0006,
	ARV_BUFFER_PAYLOAD_TYPE_JPEG2000 = 0x0007,
	ARV_BUFFER_PAYLOAD_TYPE_H264 = 0x0008,
	ARV_BUFFER_PAYLOAD_TYPE_MULTIZONE_IMAGE = 0x0009
} BufferPayloadType;
typedef unsigned int ArvPixelFormat;
typedef struct _ArvBufferPrivate {
	size_t size;
	bool is_preallocated;
	void *data;

	BufferStatus status;

	BufferPayloadType payload_type;

	unsigned int chunk_endianness;

	unsigned int frame_id;
	unsigned long long timestamp_ns;//时间戳
	unsigned long long system_timestamp_ns;//系统时间戳

	unsigned int x_offset;
	unsigned int y_offset;
	unsigned int width;
	unsigned int height;

	ArvPixelFormat pixel_format;
} ArvBuffer;

class ImageBuffer
{
public:
	ImageBuffer();
	~ImageBuffer();

	ArvBuffer *			arv_buffer_new_allocate(size_t size);
	ArvBuffer *			arv_buffer_new(size_t size, void *preallocated);
	//ArvBuffer * 		arv_buffer_new_full(size_t size, void *preallocated,void *user_data, GDestroyNotify user_data_destroy_func);
	BufferStatus		arv_buffer_get_status(ArvBuffer *buffer);
	const void *		arv_buffer_get_user_data(ArvBuffer *buffer);
	BufferPayloadType arv_buffer_get_payload_type(ArvBuffer *buffer);
	unsigned long long	arv_buffer_get_timestamp(ArvBuffer *buffer);
	void				arv_buffer_set_timestamp(ArvBuffer *buffer, unsigned long long timestamp_ns);
	unsigned long long	arv_buffer_get_system_timestamp(ArvBuffer *buffer);
	void				arv_buffer_set_system_timestamp(ArvBuffer *buffer, unsigned long long timestamp_ns);
	unsigned int 		arv_buffer_get_frame_id(ArvBuffer *buffer);
	const void			*arv_buffer_get_data(ArvBuffer *buffer, size_t &size);
	void				arv_buffer_get_image_region(ArvBuffer *buffer, int &x, int &y, int &width, int &height);
	int					arv_buffer_get_image_width(ArvBuffer *buffer);
	int					arv_buffer_get_image_height(ArvBuffer *buffer);
	int					arv_buffer_get_image_x(ArvBuffer *buffer);
	int					arv_buffer_get_image_y(ArvBuffer *buffer);
	ArvPixelFormat		arv_buffer_get_image_pixel_format(ArvBuffer *buffer);
	const void			*arv_buffer_get_chunk_data(ArvBuffer *buffer, unsigned long long chunk_id, size_t &size);
};

#endif
