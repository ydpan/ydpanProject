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
	GVCPDevice m_gvcpDevice;//本机地址 port 3956
	GVCPDevice m_DiscoveryDevice;//?? IP地址为255.255.255.255 port 3956
	GVSPDevice m_gscpDevice;//初始化 本机地址 port 0


	boost::asio::ip::address m_controlAddress;
	unsigned long long m_controller_time;


	//thread 线程
};

#endif
