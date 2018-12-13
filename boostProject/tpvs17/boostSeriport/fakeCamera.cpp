#include "fakeCamera.h"

#define ARV_GV_FAKE_CAMERA_BUFFER_SIZE	65536
typedef struct {
	void *ImageBuffer;
	size_t size;
}GInputVector;
fakeCamera::fakeCamera()
{
}


fakeCamera::~fakeCamera()
{
}

void fakeCamera::arv_gv_fake_camera_new()
{
	//create camera 
}

bool fakeCamera::arv_gv_fake_camera_start()
{
	
	//1、获取系统IP

	//2、设备信息初始化
	m_deviceInfo.Init();
	//3、GVCP初始化
	//4、GVSP初始化

	bStop = false;
	//开启线程循环

	return true;
}

void fakeCamera::arv_gv_fake_camera_stop()
{
	//1、socket关闭
	//
	bStop = true;
	//线程退出
}

void fakeCamera::MainLoop()//线程
{
	DeviceInfo *gv_fack_camera = NULL;
	void *image_buffer = NULL;
	void *error = NULL;
	void *stream_address = NULL;
	void *packet_buffer;
	size_t packet_size;
	size_t payload = 0;
	guint16 block_id;
	ptrdiff_t offset;
	guint32 gv_packet_size;
	GInputVector input_vector;
	int n_events;
	bool is_streaming = FALSE;

	input_vector.ImageBuffer = malloc(ARV_GV_FAKE_CAMERA_BUFFER_SIZE);
	input_vector.size = ARV_GV_FAKE_CAMERA_BUFFER_SIZE;

	packet_buffer = malloc(ARV_GV_FAKE_CAMERA_BUFFER_SIZE);

	do {
		guint64 next_timestamp_us;
		guint64 sleep_time_us;

		if (is_streaming) {
			sleep_time_us = 100;// arv_fake_camera_get_sleep_time_for_next_frame(gv_fake_camera->priv->camera, &next_timestamp_us);
		}
		else {
			sleep_time_us = 100000;
			next_timestamp_us =100/* g_get_real_time()*/ + sleep_time_us;
		}

		do {
			int timeout_ms = 0;

			timeout_ms = 100;// min(100, (next_timestamp_us - g_get_real_time()) / 1000LL);
			if (timeout_ms < 0)
				timeout_ms = 0;

// 			n_events = g_poll(gv_fake_camera->priv->gvcp_fds, 2, timeout_ms);
// 			if (n_events > 0) {
// 				GSocketAddress *remote_address = NULL;
// 				int count;
// 
// 				count = g_socket_receive_message(gv_fake_camera->priv->gvcp_socket,
// 					&remote_address, &input_vector, 1, NULL, NULL,
// 					NULL, NULL, NULL);
// 				if (count > 0) {
// 					if (handle_control_packet(gv_fake_camera, gv_fake_camera->priv->gvcp_socket,
// 						remote_address, input_vector.buffer, count))
// 						arv_debug_device("[GvFakeCamera::thread] Control packet received");
// 				}
// 				g_clear_object(&remote_address);

// 				if (gv_fake_camera->priv->discovery_socket != NULL) {
// 					count = g_socket_receive_message(gv_fake_camera->priv->discovery_socket,
// 						&remote_address, &input_vector, 1, NULL, NULL,
// 						NULL, NULL, NULL);
// 					if (count > 0) {
// 						if (handle_control_packet(gv_fake_camera, gv_fake_camera->priv->discovery_socket,
// 							remote_address, input_vector.buffer, count))
// 							arv_debug_device("[GvFakeCamera::thread]"
// 								" Control packet received on discovery socket\n");
// 					}
// 					g_clear_object(&remote_address);
// 				}
//			}
		} while (1/*!g_atomic_int_get(&gv_fake_camera->priv->cancel) && g_get_real_time() <*/ +next_timestamp_us);

		if (m_deviceInfo.GetCCP() ==0 || m_deviceInfo.GetAcquisitionState() ==0
// 			arv_fake_camera_get_control_channel_privilege(gv_fake_camera->priv->camera) == 0 ||
// 			arv_fake_camera_get_acquisition_status(gv_fake_camera->priv->camera) == 0
			) {
			if (stream_address != NULL) {

				stream_address = NULL;

				image_buffer = NULL;
				cout<<"[GvFakeCamera::thread] Stop stream";
			}
			is_streaming = FALSE;
		}
		else {
			if (stream_address == NULL) {
				/*
				获取数据流地址，获取图像数据
				*/
// 				GInetAddress *inet_address;
// 				char *inet_address_string;
// 获取要发送数据的IP地址和端口号
				m_deviceInfo.GetDestinationAddress();//IP
				m_deviceInfo.GetDestinationPort();//端口
				
// 				stream_address = arv_fake_camera_get_stream_address(gv_fake_camera->priv->camera);
// 				inet_address = g_inet_socket_address_get_address
// 				(G_INET_SOCKET_ADDRESS(stream_address));
// 				inet_address_string = g_inet_address_to_string(inet_address);
// 				arv_debug_stream_thread("[GvFakeCamera::thread] Start stream to %s (%d)",
// 					inet_address_string,
// 					g_inet_socket_address_get_port
// 					(G_INET_SOCKET_ADDRESS(stream_address)));
// 				g_free(inet_address_string);
// 
// 				payload = arv_fake_camera_get_payload(gv_fake_camera->priv->camera);
// 				image_buffer = arv_buffer_new(payload, NULL);
			}
			//设置相机曝光、增益等参数、触发相机 获取图像数据 返回数据大小
			//arv_fake_camera_fill_buffer(gv_fake_camera->priv->camera, image_buffer, &gv_packet_size);

			//arv_debug_stream_thread("[GvFakeCamera::thread] Send frame %d", image_buffer->priv->frame_id);

			block_id = 0;

			packet_size = ARV_GV_FAKE_CAMERA_BUFFER_SIZE;
			//打包leader数据头
		m_gscpDevice.arv_gvsp_packet_new_data_leader(1/*image_buffer->priv->frame_id*/,
				block_id,
				/*image_buffer->priv->timestamp_ns*/1,
				/*image_buffer->priv->pixel_format*/1,
				/*image_buffer->priv->width*/1, /*image_buffer->priv->height*/1,
				/*image_buffer->priv->x_offset*/1, /*image_buffer->priv->y_offset*/1,
				packet_buffer, &packet_size);
		//发送数据
// 			g_socket_send_to(gv_fake_camera->priv->gvsp_socket, stream_address,
// 				packet_buffer, packet_size, NULL, &error);
// 			if (error != NULL) {
// 				arv_warning_stream_thread("[GvFakeCamera::thread] Failed to send leader for frame %d: %s",
// 					image_buffer->priv->frame_id, error->message);
// 				g_clear_error(&error);
// 			}

			block_id++;

			offset = 0;
			while (offset < payload) {
				size_t data_size;

				data_size = 100;// min(gv_packet_size - ARV_GVSP_PACKET_PROTOCOL_OVERHEAD,payload - offset);

				packet_size = ARV_GV_FAKE_CAMERA_BUFFER_SIZE;
				m_gscpDevice.arv_gvsp_packet_new_data_block(1/*image_buffer->priv->frame_id*/, block_id,
					data_size, NULL/*((char *)image_buffer->priv->data) + offset*/,
					packet_buffer, &packet_size);

				//数据发送
// 				g_socket_send_to(gv_fake_camera->priv->gvsp_socket, stream_address,
// 					packet_buffer, packet_size, NULL, &error);
// 				if (error != NULL) {
// 					arv_debug_stream_thread("[GvFakeCamera::thread] Failed to send frame block %d for frame: %s",
// 						block_id,
// 						image_buffer->priv->frame_id,
// 						error->message);
// 					g_clear_error(&error);
// 				}

				offset += data_size;
				block_id++;
			}

			packet_size = ARV_GV_FAKE_CAMERA_BUFFER_SIZE;
			//trailer数据打包
			m_gscpDevice.arv_gvsp_packet_new_data_trailer(1/*image_buffer->priv->frame_id*/, block_id,
				packet_buffer, &packet_size);
			//发送trailer数据
// 			g_socket_send_to(gv_fake_camera->priv->gvsp_socket, stream_address,
// 				packet_buffer, packet_size, NULL, &error);
// 			if (error != NULL) {
// 				arv_debug_stream_thread("[GvFakeCamera::thread] Failed to send trailer for frame %d: %s",
// 					image_buffer->priv->frame_id,
// 					error->message);
// 				g_clear_error(&error);
// 			}

			is_streaming = TRUE;
		}

	} while (bStop/*!g_atomic_int_get(&gv_fake_camera->priv->cancel)*/);

// 	if (stream_address != NULL)
// 		g_object_unref(stream_address);
// 	if (image_buffer != NULL)
// 		g_object_unref(image_buffer);
// 
// 	g_free(packet_buffer);
// 	g_free(input_vector.buffer);

//	return NULL;
}
