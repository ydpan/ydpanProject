// UdpbroastSender.cpp : 定义控制台应用程序的入口点。
//Udpbroast Sender
 
#include "stdafx.h"
#include "UdpLinkServer.h"
#include <boost/thread/thread.hpp>
 
 
//设备信息
typedef struct tagDeviceInfo{
	unsigned short	usFunction;					//功能码
	unsigned short	usVersionFlag;				//版本标记
	unsigned int	uiCompanyId;				//企业id
	char			szDeviceSerialNo[24];		//设备序列号
	unsigned short	usServicePort;				//数据服务端口
	char			szExtend[38];				//扩展
}DeviceInfo;
 
//工作线程参数
typedef struct tagWorkThreadParameter{
	boost::asio::io_service * pIoService;
	UdpLinkServer * pUdpService;
}WorkThreadParameter;
 
//工作线程函数处理函数
bool g_WorkThreadExit = false;
int  g_nBroastDataSendInteral = 5000;					//广播发送间隔，单位：毫秒
DeviceInfo g_diDeviceInfo = {0};						//设备信息
unsigned int __stdcall WorkThreadFunByDeviceServiceProcess(PVOID pParam)
{
	int nn = 0;
	int nDataSize = sizeof(DeviceInfo);
	WorkThreadParameter *pAllParameter = (WorkThreadParameter *)pParam;
	while(true)
	{
		if(g_WorkThreadExit)
		{
			break;
		}
 
		//发送数据
		pAllParameter->pUdpService->SendData((char *)&g_diDeviceInfo,nDataSize,true);
 
		pAllParameter->pIoService->poll();
		for(nn=g_nBroastDataSendInteral;nn>0; nn-=200)
		{
			if(g_WorkThreadExit)
			{
				break;
			}
			Sleep(200);
		}
	}
	return 0;
}
 
int main(int argc, char* argv[])
{
	boost::asio::io_service ioService;
	UdpLinkServer usUdpService(9090,true);//true，为广播模式；false，非广播模式
	usUdpService.Start(ioService);
	g_diDeviceInfo.usFunction = 1;
	g_diDeviceInfo.usVersionFlag = 0x0001;
	strcpy(g_diDeviceInfo.szDeviceSerialNo,"ABCDEFG111111111");
	g_diDeviceInfo.usServicePort = 9001;
	
	WorkThreadParameter wtpWorkThreadParameter;
	wtpWorkThreadParameter.pIoService = &ioService;
	wtpWorkThreadParameter.pUdpService = &usUdpService;
	//开启线程
	boost::thread thrd(WorkThreadFunByDeviceServiceProcess,&wtpWorkThreadParameter);
	//线程poll等待
	thrd.join();
	g_WorkThreadExit = true;
 
	usUdpService.Stop();
	ioService.stop();
	return 0;
}
