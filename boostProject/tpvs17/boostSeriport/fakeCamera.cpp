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
	
	//1����ȡϵͳIP

	//2���豸��Ϣ��ʼ��
	m_pdeviceInfo->Init();
	//3��GVCP��ʼ��
	//4��GVSP��ʼ��

	bStop = false;

	//�����߳�ѭ��
	boost::function0<void> f = boost::bind(&FakeCamera::MainLoop, this);
	boost::thread thrd(f);
	thrd.join();
	return true;
}

void FakeCamera::camera_stop()
{
	//1��socket�ر�
	//
	bStop = true;
	//�߳��˳�
}

void FakeCamera::MainLoop()//�߳�
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
				//��ʱ�ж�
				{
					uint64_t time = m_pdeviceInfo->getcurrentTime();
					uint64_t elapsed_ms = (time - m_pdeviceInfo->getCtroller_time()) / 1000;
					uint32_t ntimeout = m_pdeviceInfo->GetHeartbeatTimeout();
					if (elapsed_ms > ntimeout)
					{
						//��ʱ
						//���� 
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
				��ȡ��������ַ����ȡͼ������
				*/
				// ��ȡҪ�������ݵ�IP��ַ�Ͷ˿ں�
			std::string strAddr = numtoIp(m_pdeviceInfo->GetDestinationAddress());//IP
			if (strAddr == "0.0.0.0")
				strAddr = "127.0.0.1";
			uint32_t nPort = m_pdeviceInfo->GetDestinationPort();//�˿�
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
			//���leader����ͷ
			tagUdpData sendLeaderData;
			sendLeaderData.fromPoint = targetEndpoint;
			m_pGVSPDevice->gvsp_packet_data_leader(image_buffer->frame_id,
				block_id,
				image_buffer->timestamp_ns,
				image_buffer->pixel_format,
				image_buffer->width, image_buffer->height,
				image_buffer->x_offset, image_buffer->y_offset,
				sendLeaderData);
			m_pGVSPDevice->sendData(sendLeaderData);//��������
		
			{
				//TODO ����ʧ�ܵ�һЩ�������ݴ���
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
				m_pGVSPDevice->sendData(sendblockData);//blob���ݷ���
				
				{
					//TODO ����ʧ�ܵ�һЩ�������ݴ���
				}
				offset += data_size;
				block_id++;
			}


			//trailer���ݴ��
			tagUdpData sendtrailerData;
			sendtrailerData.fromPoint = targetEndpoint;
			m_pGVSPDevice->gvsp_packet_data_trailer(image_buffer->frame_id, block_id,sendtrailerData);
			m_pGVSPDevice->sendData(sendtrailerData);//����trailer����
			{
				//TODO ����ʧ�ܵ�һЩ�������ݴ���
			}

			is_streaming = TRUE;
		}
	} while (!bStop/*!g_atomic_int_get(&gv_fake_camera->priv->cancel)*/);

	{
		//TODO �˳��̵߳�һ�����ݴ���
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

