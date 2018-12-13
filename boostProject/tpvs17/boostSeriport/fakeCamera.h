#ifndef _FACKCAMERA_H_
#define _FACKCAMERA_H_
#include <string>
#include <boost/asio.hpp>
#include "DeviceInfo.h"
#include "GVCPDevice.h"
#include "GVSPDevice.h"

class fakeCamera
{
public:
	fakeCamera();
	~fakeCamera();

	void arv_gv_fake_camera_new();
	bool arv_gv_fake_camera_start();
	void arv_gv_fake_camera_stop();
	void MainLoop();
private:
	bool bInit{ false };
	bool bStop{ false };

	DeviceInfo m_deviceInfo;
	GVCPDevice m_gvcpDevice;//������ַ port 3956
	GVCPDevice m_DiscoveryDevice;//?? IP��ַΪ255.255.255.255 port 3956
	GVSPDevice m_gscpDevice;//��ʼ�� ������ַ port 0


	boost::asio::ip::address m_controlAddress;
	unsigned long long m_controller_time;


	//thread �߳�
};

#endif
