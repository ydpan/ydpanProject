#include "ImageBuffer.h"
#include <lmcons.h>

ImageBuffer::ImageBuffer()
{

}

ImageBuffer::~ImageBuffer()
{

}

ArvBuffer *ImageBuffer::arv_buffer_new_full(size_t size, void *preallocated)
{
	ArvBuffer *buffer;

	buffer = new ArvBuffer();
	buffer->size = size;
	buffer->chunk_endianness = 0;
	buffer->payload_type = ARV_BUFFER_PAYLOAD_TYPE_UNKNOWN;

	if (preallocated) {
		buffer->is_preallocated = true;
		buffer->data = preallocated;
	}
	else {
		buffer->is_preallocated = false;
		//buffer->data = new c(size);//分配内存
	}

	return buffer;
}

ArvBuffer *ImageBuffer::arv_buffer_new(size_t size, void *preallocated)
{
	return  arv_buffer_new_full(size, preallocated);
}

const void *ImageBuffer::arv_buffer_get_data(ArvBuffer *buffer, size_t &size)
{
	if (buffer == NULL)
		return NULL;

	size = buffer->size;

	return buffer->data;
}

//指定数量的数据
const void *ImageBuffer::arv_buffer_get_chunk_data(ArvBuffer *buffer, unsigned long long chunk_id, size_t &size)
{
	ArvChunkInfos *infos;
	char *data;
	size_t offset;
	if (buffer == NULL)
		return NULL;
	if (buffer->data == NULL)
		return NULL;
	if (buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_CHUNK_DATA ||
		buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA)
		return NULL;

	if (buffer->status != ARV_BUFFER_STATUS_SUCCESS)
		return NULL;

	data = (char*)buffer->data;
	offset = buffer->size - sizeof(ArvChunkInfos);
	while (offset > 0) {
		unsigned int id = 0;
		unsigned int chunk_size = 0;

		infos = (ArvChunkInfos *)&data[offset];

		if (buffer->chunk_endianness == 0) {
			id = infos->id;
			chunk_size = infos->size;
		}
		else {
 			id = infos->id;
 			chunk_size = infos->size;
		}

		if (id == chunk_id) {
			size_t data_offset;

			data_offset = offset - chunk_size;
			if (data_offset >= 0) {
				size = chunk_size;
				return &data[data_offset];
			}
			else
				return NULL;
		}
		if (chunk_size > 0)
			offset = offset - chunk_size - sizeof(ArvChunkInfos);
		else
			offset = 0;
	}

	return NULL;
}

BufferStatus ImageBuffer::arv_buffer_get_status(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return ARV_BUFFER_STATUS_UNKNOWN;
	return buffer->status;
}

BufferPayloadType ImageBuffer::arv_buffer_get_payload_type(ArvBuffer *buffer)
{
	if(buffer==NULL)
		return ARV_BUFFER_PAYLOAD_TYPE_UNKNOWN;

	return buffer->payload_type;
}

unsigned long long ImageBuffer::arv_buffer_get_timestamp(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return 0;

	return buffer->timestamp_ns;
}

void ImageBuffer::arv_buffer_set_timestamp(ArvBuffer *buffer, unsigned long long timestamp_ns)
{
	if (buffer == NULL)
		return ;

	buffer->timestamp_ns = timestamp_ns;
}

unsigned long long ImageBuffer::arv_buffer_get_system_timestamp(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return 0;

	return buffer->system_timestamp_ns;
}

void ImageBuffer::arv_buffer_set_system_timestamp(ArvBuffer *buffer, unsigned long long timestamp_ns)
{
	if (buffer == NULL)
		return ;

	buffer->system_timestamp_ns = timestamp_ns;
}

unsigned int ImageBuffer::arv_buffer_get_frame_id(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return 0;

	return buffer->frame_id;
}

void ImageBuffer::arv_buffer_get_image_region(ArvBuffer *buffer, int &x, int &y, int &width, int &height)
{
	if (buffer == NULL)
		return;
	if (buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_IMAGE || 
		buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA)
			return ;
	x = buffer->x_offset;
	y = buffer->y_offset;
	width = buffer->width;
	height = buffer->height;
}

int ImageBuffer::arv_buffer_get_image_width(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return 0;

	if(buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_IMAGE ||
 	   buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA)
	return 0;

	return buffer->width;
}

int ImageBuffer::arv_buffer_get_image_height(ArvBuffer *buffer)
{
	if(buffer==NULL)
		return  0;
	if (buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_IMAGE ||
		buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA)
		return 0;

	return buffer->height;
}

int ImageBuffer::arv_buffer_get_image_x(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return  0;
	if (buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_IMAGE ||
		buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA)
		return 0;
	return buffer->x_offset;
}

int ImageBuffer::arv_buffer_get_image_y(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return  0;
	if (buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_IMAGE ||
		buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA)
		return 0;
	return buffer->y_offset;
}

ArvPixelFormat ImageBuffer::arv_buffer_get_image_pixel_format(ArvBuffer *buffer)
{
	if (buffer == NULL)
		return  0;
	if (buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_IMAGE ||
		buffer->payload_type == ARV_BUFFER_PAYLOAD_TYPE_EXTENDED_CHUNK_DATA)
		return 0;
	return buffer->pixel_format;
}

