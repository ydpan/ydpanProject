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

class UDPParams {
public:
	string serialPort{ "/dev/ttyUSB0" };//设备名
	unsigned int baudRate{ 115200 };//波特率
	unsigned int flowControl{ 0 };//流控
	unsigned int parity{ 0 };//校验位
	unsigned int stopBits{ 0 };//停止位
	UDPParams()
	{
	}

	UDPParams(
		string _serialPort, 
		unsigned int _baudRate,
		unsigned int  _flowControl,
		unsigned int  _parity,
		unsigned int  _stopBits
	)
	{
		serialPort = _serialPort;
		baudRate = _baudRate;
		flowControl = _flowControl;
		parity = _parity;
		stopBits = _stopBits;
	}
};

typedef boost::shared_ptr<ByteVector> pByteVector;
typedef boost::shared_ptr<DataGram> DataGramPtr;
class UDPDevice
{
public:
	UDPDevice();
	virtual ~UDPDevice();

	void setSerialParams(const UDPParams &params);
	void setTimeOut(int timeout = 3000);
	bool startThread();
	bool stopThread();

	void setCallbackFunc(const boost::function<void(DataGramPtr)> &func);
	bool writeDataGram(const DataGram &datagram);
	bool writeRaw(const ByteVector &rawData);
private:
	boost::shared_ptr<deadline_timer> m_ptimer;//超时定时器
	boost::shared_ptr<io_service> m_pios;//io_service 
	boost::shared_ptr<serial_port> m_pSerial; //串口对象
	mutex m_serialMutex;//串口对象锁 

	enum{HEADER_LEN=4};
	enum REC_STATE {
		WAITING_FF1, WAITING_FF2, READING_HEAD, READING_DATA, READING_CHECKSUM
	}m_state;

	UDPParams	m_serialParams;//设备属性数据
	int					m_timeOut{ 1000 };//超时时间

	ByteVector			m_tempBuf;//数据读取缓冲区
	
	ByteVector			m_currentHeader;//报头
	size_t				m_headerBytesRead;

	ByteVector			m_currentData;//读取数据
	size_t				m_DataBytesRead;//读取字节数

	queue<pByteVector>	m_writeQueue;//待发送数据队列
	mutex				m_writeQueueMutex;//队列互斥锁

	boost::function<void(DataGramPtr)> m_dataCallbackFunc;//数据回调函数
	boost::function<void()> m_errorCallbackFunc;//错误回调函数

	boost::thread m_thread;//io_service::run线程用

	void mainRun();

	void start_a_read();
	void start_a_write();

	void readHandler(const system::error_code &ec, size_t bytesTransferred);
	void writeHandler(const system::error_code &ec);
	void timeoutHandler(const system::error_code &ec);

};

std::string make_daytime_string()
{
	std::time_t now = time(0);
	return ctime(&now);
}
class UdpTimeServer 
{
public:
	UdpTimeServer(boost::asio::io_service &io_service):_sockUdp(io_service,udp::endpoint(udp::v4(),13))
	{
		RecvTime();
	}
private:
	void RecvTime(void) 
	{//接收收客户端的请求
		_sockUdp.async_receive_from(
			boost::asio::buffer(_recvBuf), _endpointRemote,
			boost::bind(&UdpTimeServer::handleRecvTime, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	void handleSendTime(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t)
	{//当发送时间字符串给客户端成功之后响应。

	}
	void handleRecvTime(const boost::system::error_code& error, std::size_t) 
	{
		//当收到客户端数据时，就进入本函数响应处理
		if (!error || error == boost::asio::error::message_size)
		{
			boost::shared_ptr<std::string> strMessage(new std::string(make_daytime_string()));
			_sockUdp.async_send_to(boost::asio::buffer(*strMessage), _endpointRemote,
				boost::bind(&UdpTimeServer::handleSendTime, this, strMessage,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

			RecvTime();
		}
	}
private:
	udp::socket _sockUdp;
	udp::endpoint _endpointRemote;
	boost::array<char, 1> _recvBuf;
};
/*
void TestUdp(void)
{
	boost::asio::io_service io_service;
	UdpTimeServer udpTimeServer(io_service);
	io_service.run();
}*/
#endif // _SERIALPORT_H_
