#ifndef _UDPSERVER_H_
#define _UDPSERVER_H_

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <inttypes.h>
#include "DataGram.h"
using namespace std;
using namespace boost;
using namespace boost::asio;
using  boost::asio::ip::udp;

typedef boost::shared_ptr<ByteVector> pByteVector;
typedef struct tagUdpData{// 包含接收数据的源端口及数据
	boost::asio::ip::udp::endpoint fromPoint;
	pByteVector _byteData;
}UDPData;

class UDPService
{
public:
	/*UDPService();*/
	UDPService(int port = 13);
	virtual ~UDPService();

	void setTimeOut(int timeout = 3000);
	bool startThread();
	bool stopThread();
	/*
		通过该种方式调用 绑定回调函数
	setCallbackFunc(boost::bind(&boostSeriport::serialCallback,this,_1));
	*/
	void setCallbackFunc(const boost::function<void(tagUdpData)> &func);

	bool writeRaw(const ByteVector &rawData);
	bool writeRaw(const tagUdpData &tagData);
	bool writeRaw(const boost::asio::ip::udp::endpoint _endpoint, const ByteVector &rawData);
private:
	boost::shared_ptr<deadline_timer> m_ptimer;//超时定时器
	io_service m_pios;//io_service 
	mutex m_udpMutex;//对象锁 

	int					m_timeOut{ 1000 };//超时时间

	int					m_bufSize{ 1024 };//默认缓冲区大小
	ByteVector			m_tempBuf;//数据读取缓冲区
	
	queue<tagUdpData>	m_writeQueue;//待发送数据队列
	mutex				m_writeQueueMutex;//队列互斥锁

	boost::function<void(tagUdpData)> m_dataRecvCallBackFunc;//数据回调函数
	boost::function<void()> m_errorCallbackFunc;//错误回调函数

	boost::thread m_thread;//io_service::run线程用

	void mainRun();

	void readOnce();//读一次
	void writeOnce();//写一次

	void readHandler(const system::error_code &ec, size_t bytesTransferred);
	void writeHandler(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t);
	void timeoutHandler(const system::error_code &ec);


	udp::socket _socket;
	udp::endpoint _endpointRemote;//远程发送数据的端点  接收数据时获取 IP及端口号
};

#endif // _SERIALPORT_H_
