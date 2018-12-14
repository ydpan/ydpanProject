#include "FakeCamera.h"
#include <fstream>

#define ARV_GV_FAKE_CAMERA_BUFFER_SIZE	65536
#define IMAGE_FILE_MAX_SIZE 4 << 20

int min(int x, int y) {
	if (x > y)
		return y;
	else
		return x;
}

typedef struct {
	void *ImageBuffer;
	size_t size;
}GInputVector;

std::string numtoIp(unsigned int ipNum)
{
	std::string strIP;
	unsigned char *val = (unsigned char *)&ipNum;
	char *p = (char*)strIP.c_str();
	sprintf(p, "%u.%u.%u.%u", val[3], val[2], val[1], val[0]);
	return strIP;
}

FakeCamera::FakeCamera()
{
	image_buffer = new ArvBuffer();
}


FakeCamera::~FakeCamera()
{
	if (m_pdeviceInfo)
	{
		delete m_pdeviceInfo;
		m_pdeviceInfo = NULL;
	}
}

void FakeCamera::Init()
{
	m_pdeviceInfo = new DeviceInfo();
	m_pGVCPD = boost::make_shared<GVCPDevice>(m_pdeviceInfo);
	m_pGVSPDevice = boost::make_shared<GVSPDevice>();
	m_pGVCPD->InitDevice();
}

bool FakeCamera::camera_start()
{
	
	//1、获取系统IP

	//2、设备信息初始化
	m_pdeviceInfo->Init();
	//3、GVCP初始化
	//4、GVSP初始化

	bStop = false;

	//开启线程循环
	boost::function0<void> f = boost::bind(&FakeCamera::MainLoop, this);
	boost::thread thrd(f);
	thrd.join();
	return true;
}

void FakeCamera::camera_stop()
{
	//1、socket关闭
	//
	bStop = true;
	//线程退出
}

void FakeCamera::MainLoop()//线程
{
	DeviceInfo *gv_fack_camera = NULL;
	void *error = NULL;
	void *stream_address = NULL;
	void *packet_buffer;
	size_t packet_size=0;
	size_t payload = 0;
	guint16 block_id;
	long long offset;
 	bool is_streaming = FALSE;

	
	do {
		guint64 next_timestamp_us;
		guint64 sleep_time_us;

		if (is_streaming) {
			sleep_time_us = m_pdeviceInfo->Get_Sleep_time_for_next_frame(next_timestamp_us);
		}
		else {
			sleep_time_us = 100000;
			next_timestamp_us = m_pdeviceInfo->getcurrentTime() + sleep_time_us;
		}

		boost::this_thread::sleep(boost::posix_time::seconds(1));//boost::get_system_time() + 
		std::string strAdr = m_pdeviceInfo->controler_address.to_string();
		if (strAdr != "0.0.0.0")
		{
		do {
				int timeout_ms = 0;
				timeout_ms = min(100, (next_timestamp_us - m_pdeviceInfo->getcurrentTime()) / 1000);
				if (timeout_ms < 0)
					timeout_ms = 0;
				//超时判断
				{
					uint64_t time = m_pdeviceInfo->getcurrentTime();
					uint64_t elapsed_ms = (time - m_pdeviceInfo->getCtroller_time()) / 1000;
					uint32_t ntimeout = m_pdeviceInfo->GetHeartbeatTimeout();
					if (elapsed_ms > ntimeout)
					{
						//超时
						//重置 
						m_pdeviceInfo->SetControlChannelPrivilege(1);
					}
					
				}

			} while (!bStop&& m_pdeviceInfo->getcurrentTime() < next_timestamp_us);
		}
		m_pdeviceInfo->SetControlChannelPrivilege(1);
		if (m_pdeviceInfo->GetControlChannelPrivilege() ==0/* || m_pdeviceInfo->GetAcquisitionStatus() ==0*/)
		{
			if (stream_address != NULL) {

				stream_address = NULL;
				image_buffer = NULL;
				cout<<"[GvFakeCamera::thread] Stop stream";
			}
			is_streaming = FALSE;
		}
		else 
		{
			//continue;
			//if (stream_address == NULL)
			//{
				/*
				获取数据流地址，获取图像数据
				*/
				// 获取要发送数据的IP地址和端口号
			std::string strAddr = numtoIp(m_pdeviceInfo->GetDestinationAddress());//IP
			if (strAddr == "0.0.0.0")
				strAddr = "127.0.0.1";
			uint32_t nPort = m_pdeviceInfo->GetDestinationPort();//端口
			if (nPort == 0)
				nPort = 21200;
			boost::asio::ip::udp::endpoint targetEndpoint(boost::asio::ip::address::from_string(strAdr), nPort);

			{
				image_buffer->size = m_pdeviceInfo->GetPayload();
				image_buffer->status = ARV_BUFFER_STATUS_SUCCESS;
				image_buffer->payload_type = ARV_BUFFER_PAYLOAD_TYPE_IMAGE;
				image_buffer->timestamp_ns = m_pdeviceInfo->getcurrentTime() / 1000;
				image_buffer->system_timestamp_ns = m_pdeviceInfo->getcurrentTime() / 1000;
				image_buffer->frame_id = image_buffer->frame_id++;
				image_buffer->pixel_format = m_pdeviceInfo->GetRegisterData(ARV_FAKE_CAMERA_REGISTER_PIXEL_FORMAT);
			}
			image_buffer->pixel_format = ARV_PIXEL_FORMAT_MONO_8;


			block_id = 0;
			packet_size = ARV_GV_FAKE_CAMERA_BUFFER_SIZE;
			//打包leader数据头
			tagUdpData sendLeaderData;
			sendLeaderData.fromPoint = targetEndpoint;
			m_pGVSPDevice->gvsp_packet_data_leader(image_buffer->frame_id,
				block_id,
				image_buffer->timestamp_ns,
				image_buffer->pixel_format,
				image_buffer->width, image_buffer->height,
				image_buffer->x_offset, image_buffer->y_offset,
				sendLeaderData);
			m_pGVSPDevice->sendData(sendLeaderData);//发送数据
		
			{
				//TODO 发送失败的一些基本数据处理
			}

			block_id++;
			offset = 0;
			tagUdpData sendblockData;
			sendblockData.fromPoint = targetEndpoint;
			payload = image_buffer->size;
			uint32_t sendpackit = 512;// m_pdeviceInfo->GetStream_Channel_0_Packet_Size();
			while (offset < payload) {
				size_t data_size;

				data_size = min(sendpackit - ARV_GVSP_PACKET_PROTOCOL_OVERHEAD,payload - offset);

				packet_size = ARV_GV_FAKE_CAMERA_BUFFER_SIZE;
				m_pGVSPDevice->gvsp_packet_data_block(image_buffer->frame_id, block_id,
					data_size, ((char *)image_buffer->data) + offset,
					sendblockData);
				m_pGVSPDevice->sendData(sendblockData);//blob数据发送
				
				{
					//TODO 发送失败的一些基本数据处理
				}
				offset += data_size;
				block_id++;
			}


			//trailer数据打包
			tagUdpData sendtrailerData;
			sendtrailerData.fromPoint = targetEndpoint;
			m_pGVSPDevice->gvsp_packet_data_trailer(image_buffer->frame_id, block_id,sendtrailerData);
			m_pGVSPDevice->sendData(sendtrailerData);//发送trailer数据
			{
				//TODO 发送失败的一些基本数据处理
			}

			is_streaming = TRUE;
		}
	} while (!bStop/*!g_atomic_int_get(&gv_fake_camera->priv->cancel)*/);

	{
		//TODO 退出线程的一般数据处理
	}
}

void FakeCamera::ReadImageData()
{
 	string strpath = "E:\\LeaperProject\\ydpanProject\\1.png";
// 	{
// 		image.load(strpath.c_str());
// 		_nImageLen= image.byteCount();
// 		_nSizeX = image.width();
// 		_nSizeY = image.height();
// 		_pImageData = image.bits();
// 	}
	std::fstream file;
	_pImageData = new unsigned char[IMAGE_FILE_MAX_SIZE];
	file.open(strpath.c_str(), ios::in | ios::binary);

	if (file.is_open())
	{
		file.seekg(0, ios::end);
		_nImageLen = file.tellg();
		file.seekg(0, ios::beg);
		if (_nImageLen <= IMAGE_FILE_MAX_SIZE)
		{
			_nSizeX = _nSizeY = 100;
			file.read((char*)_pImageData, _nImageLen);
			file.close();
		}
		else
		{
			_nImageLen = 0;
			_pImageData = NULL;
			_nSizeX = _nSizeY = 0;
		}
	}
	image_buffer->data = _pImageData;
	image_buffer->frame_id = 1;
	image_buffer->height = 512;
	image_buffer->size = 512 * 512;// _nImageLen;
	image_buffer->width = 512;
	image_buffer->x_offset = 0;
	image_buffer->y_offset = 0;
	image_buffer->pixel_format = 1;
}

