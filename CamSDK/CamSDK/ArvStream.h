#pragma once
#include "ImageBuffer.h"
typedef enum ArvStreamCallbackType {
	ARV_STREAM_CALLBACK_TYPE_INIT,//thread initialization, happens once
	ARV_STREAM_CALLBACK_TYPE_EXIT,//thread end, happens once
	ARV_STREAM_CALLBACK_TYPE_START_BUFFER,//buffer filling start, happens at each frame
	ARV_STREAM_CALLBACK_TYPE_BUFFER_DONE,//buffer filled, happens at each frame
}_ArvStreamCallbackType;
void(*ArvStreamCallback) (void *user_data,ArvStreamCallbackType type,ArvBuffer *buffer);
class ArvStream
{
public:
	ArvStream();
	~ArvStream();

	void(*ArvStreamCallback) ();
	void	arv_stream_push_buffer();
	ArvBuffer *	arv_stream_pop_buffer();
	ArvBuffer *	arv_stream_try_pop_buffer();
	ArvBuffer *	arv_stream_timeout_pop_buffer();
	void	arv_stream_get_n_buffers();
	void	arv_stream_get_statistics();
	bool	arv_stream_get_emit_signals();
	void	arv_stream_set_emit_signals();
	bool	arv_make_thread_realtime();
	bool	arv_make_thread_high_priority();
};

