// UdpbroastRevicer.cpp : 定义控制台应用程序的入口点。
//Udpbroast Revicer
 
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
 
//工作线程函数处理函数
bool g_WorkThreadExit = false;
int  g_nBroastDataSendInteral = 5000;					//广播发送间隔，单位：毫秒
DeviceInfo g_diDeviceInfo = {0};						//设备信息
unsigned int __stdcall WorkThreadFunByDeviceServiceProcess(PVOID pParam)
{
	int nn = 0;
	boost::asio::io_service * pIoService  = (boost::asio::io_service *)pParam;
	while(true)
	{
		if(g_WorkThreadExit)
		{
			break;
		}
 
		pIoService->poll();
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
 
static void WINAPI BroastDeviceInfoRecvDataCallback(const boost::system::error_code& error,char *pData,int nDataLength,char *pPeerIp,unsigned short usPeerPort,DWORD dwUserData1,DWORD dwUserData2)
{
	SYSTEMTIME sm;
	GetLocalTime(&sm);
	char szInfo[256] = {0};
	DeviceInfo *pDeviceInfo = (DeviceInfo *)pData;
	sprintf(szInfo,"%s %s:%d time:%04d-%02d-%0d %02d:%02d:%02d\n",pDeviceInfo->szDeviceSerialNo,pPeerIp,usPeerPort,sm.wYear,sm.wMonth,sm.wDay,sm.wHour,sm.wMinute,sm.wSecond);
	printf(szInfo);
}
 
int main(int argc, char* argv[])
{
	boost::asio::io_service ioService;
	UdpLinkServer usUdpService(9090,false);
	usUdpService.SetRecvDataCallback(true,BroastDeviceInfoRecvDataCallback,0,0);
	usUdpService.Start(ioService);
	
	//开启线程
	boost::thread thrd(WorkThreadFunByDeviceServiceProcess,&ioService);
	//线程poll等待
	thrd.join();
	g_WorkThreadExit = true;
 
	usUdpService.Stop();
	ioService.stop();
	return 0;
}
