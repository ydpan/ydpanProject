#include "udpSender.h"

udpSender::udpSender(boost::asio::io_service &io_service):
m_socket(io_service,\
	boost::asio::ip::udp::endpoint(\
		boost::asio::ip::address::from_string("192.168.8.160"),9001))
{
	
}

udpSender::~udpSender()
{
}

bool udpSender::writeRaw(const tagUdpData &tagData)
{
	boost::shared_ptr<std::string> strMessage(new std::string((char*)(tagData._byteData->data())));
	const unsigned char * p = tagData._byteData->data();
	size_t sendSize = tagData._byteData->size();
	std::string sendMsg;
	boost::asio::ip::udp::endpoint targetpoint = tagData.fromPoint;
	std::string straddress = targetpoint.address().to_string() + ":" + to_string(targetpoint.port());
	string ss = targetpoint.address().to_string();
	m_socket.send_to(boost::asio::buffer(p, sendSize), targetpoint); 
	return true;
}
