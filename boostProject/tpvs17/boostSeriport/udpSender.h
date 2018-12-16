#ifndef UDPSENDER_H_
#define UDPSENDER_H_

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
#include "udpserver.h"
using namespace std;
using namespace boost;
using namespace boost::asio;
using  boost::asio::ip::udp;

class udpSender
{
public:
	udpSender(boost::asio::io_service &io_service);
	~udpSender();
	bool writeRaw(const tagUdpData &tagData);
private:
	boost::asio::ip::udp::socket m_socket;
};

#endif