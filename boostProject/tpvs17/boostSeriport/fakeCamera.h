#ifndef _FACKCAMERA_H_
#define _FACKCAMERA_H_
#include <string>
#include <boost/asio.hpp>
#include "DeviceInfo.h"
#include "GVCPDevice.h"
#include "GVSPDevice.h"
#include <boost/thread/thread.hpp>
#include <boost/make_shared.hpp>
#include "opencv.hpp"

using namespace cv;

class FakeCamera
{
public:
	FakeCamera();
	~FakeCamera();

	void Init();
	bool camera_start();
	void camera_stop();
	void MainLoop();

	void ReadImageData();
	void GetNextFrame(ArvBuffer *pbuffer);
private:
	bool bInit{ false };
	bool bStop{ false };

	DeviceInfo *m_pdeviceInfo{NULL};
	boost::shared_ptr<GVCPDevice> m_pGVCPD;//������ַ port 3956
	boost::shared_ptr<GVSPDevice> m_pGVSPDevice;//��ʼ�� ������ַ port 0

	boost::asio::ip::address m_controlAddress;
	unsigned long long m_controller_time;

	bool bcontrol{ false };
	//thread �߳�

	unsigned char* _pImageData{NULL};
	size_t         _nImageLen{0};
	uint32_t       _nSizeX{100};
	uint32_t       _nSizeY{100};

	//QImage image;
	ArvBuffer *image_buffer{ NULL };
	mutex	m_writeQueueMutex;//���л�����
	vector<cv::Mat> vecMat;
	cv::Mat readMat;
};

#endif
