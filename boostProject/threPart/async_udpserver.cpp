// boost_028.cpp : Defines the entry point for the console application.
//
 
#include "stdafx.h"
#include <ctime>
 
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
 
#include <iostream>
#include <string>
 
//使用UDP命名空间
using boost::asio::ip::udp;
 
//把当前时间转换为字符串。
std::string make_daytime_string()
{
	using namespace std; //为了使用time_t, time 和 ctime;
	time_t now = time(0);
	return ctime(&now);
}
 
//
//创建一个异步UDP的时间服务器。
//软件开发人员： 蔡军生  2013-08-25 
//QQ: 9073204
//
class UdpTimeServer
{
public:
	//传入IO服务，然后创建一个UDP的SOCKET，IPV4版本，端号为13
	UdpTimeServer(boost::asio::io_service& ioService)
		:m_sockUdp(ioService,  udp::endpoint(udp::v4(), 13))
	{
		//进入接收服务中。
		RecvTime();
	}
private:
	//接收收客户端的请求。
	void RecvTime(void)
	{
		//异步接收数据
		m_sockUdp.async_receive_from(
			boost::asio::buffer(m_recvBuf), m_endpointRemote,
			boost::bind(&UdpTimeServer::handleRecvTime, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
 
	//当收到客户端数据时，就进入本函数响应处理
	void handleRecvTime(const boost::system::error_code& error,
		std::size_t /*bytes_transferred*/)
	{
		//如果没有出错，就把时间字符串发送给客户端。
		if (!error || error == boost::asio::error::message_size)
		{
			boost::shared_ptr<std::string> strMessage(
				new std::string(make_daytime_string()));
 
			m_sockUdp.async_send_to(boost::asio::buffer(*strMessage), m_endpointRemote,
				boost::bind(&UdpTimeServer::handleSendTime, this, strMessage,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
 
			//接收下一次的信息。
			RecvTime();
		}
	}
 
	//当发送时间字符串给客户端成功之后响应。
	void handleSendTime(boost::shared_ptr<std::string> /*strMessage*/,
			const boost::system::error_code& /*error*/,
			std::size_t /*bytes_transferred*/)
	{
 
	}
 
private:
	udp::socket m_sockUdp; //服务器的SOCKET。
	udp::endpoint m_endpointRemote; //收到数据时的端点信息。
	boost::array<char, 1> m_recvBuf; //接收数据缓冲区。
};
 
void TestUdp(void)
{
	boost::asio::io_service ioService;
	UdpTimeServer udpTimeServer(ioService);
	ioService.run();
}
 
 
int _tmain(int argc, _TCHAR* argv[])
{
	//
	TestUdp();
 
	return 0;
}