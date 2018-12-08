#include "udpserver.h"
#include <boost/make_shared.hpp>
using namespace std;
using namespace boost;
using namespace boost::asio;
using boost::asio::ip::udp;


void TestTagfunc(tagUdpData pByV)
{
	std::string strAddress = pByV.fromPoint.address().to_string() + ":" + to_string(pByV.fromPoint.port());
	int a = 0;
}
UDPService::UDPService(int port):_socket(m_pios, udp::endpoint(boost::asio::ip::address::from_string("192.168.8.153")/*udp::v4()*/, port))
{
	m_dataRecvCallBackFunc = TestTagfunc;

	m_tempBuf.resize(m_bufSize, 0);//1024bytes
}

UDPService::~UDPService()
{
	_socket.close();
	m_pios.stop();
	m_thread.join();
	cout << "SerialPortDevice Object deleted!" << endl;
}

void UDPService::setTimeOut(int timeout /*= 3000*/)
{
	m_timeOut = timeout;
}

bool UDPService::startThread()
{
	cout << "udp::statThread() called" << endl;
	try {
		// 创建线程
		m_thread = boost::thread(boost::bind(&UDPService::mainRun, this));
	}
	catch (std::exception &e) {
		cout << "Failed to create thread !" << endl;
		return false;
	}

	return true;
}

bool UDPService::stopThread()
{
	m_pios.stop();
	return true;
}

void UDPService::setCallbackFunc(const boost::function<void(tagUdpData)> &func)
{
	m_dataRecvCallBackFunc = func;
}

bool UDPService::writeRaw(const ByteVector &rawData)
{
	mutex::scoped_lock lock(m_writeQueueMutex);
	bool writeIdle = m_writeQueue.empty();
	pByteVector data(new ByteVector(rawData));
	tagUdpData newWriteData;
	newWriteData.fromPoint = _endpointRemote;
	newWriteData._byteData = data;
	m_writeQueue.push(newWriteData);
	if (writeIdle)
	{
		writeOnce();
		return true;
	}
	return false;
}

bool UDPService::writeRaw(const tagUdpData &tagData)
{
	mutex::scoped_lock lock(m_writeQueueMutex);
	bool writeIdle = m_writeQueue.empty();
	const unsigned char * p = tagData._byteData->data();
	m_writeQueue.push(tagData);
	if (writeIdle)
	{
		writeOnce();
		return true;
	}
	return false;
}

bool UDPService::writeRaw(const boost::asio::ip::udp::endpoint _endpoint, const ByteVector &rawData)
{
	mutex::scoped_lock lock(m_writeQueueMutex);
	bool writeIdle = m_writeQueue.empty();
	pByteVector data(new ByteVector(rawData));
	tagUdpData newWriteData;
	newWriteData.fromPoint = _endpoint;
	newWriteData._byteData = data;
	m_writeQueue.push(newWriteData);
	if (writeIdle)
	{
		writeOnce();
		return true;
	}
	return false;
}

void UDPService::mainRun()
{
	cout << "udp::mainRun()"<<endl;
	//m_state = WAITING_FF1;//初始化工作状态
	readOnce();
	m_pios.run();
}

void UDPService::readOnce()
{
	cout << "udp::start_a_read() " << endl;
	mutex::scoped_lock lock(m_udpMutex);
	_socket.async_receive_from(boost::asio::buffer(m_tempBuf), _endpointRemote,
		boost::bind(&UDPService::readHandler, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void UDPService::writeOnce()
{
	if (m_writeQueue.size()>0)
	{
		cout << "udp::start_a_write() " << endl;
		mutex::scoped_lock lock(m_udpMutex);
		tagUdpData v =  m_writeQueue.front();
		
		boost::shared_ptr<std::string> strMessage(new std::string((char*)(v._byteData->data())));
		const unsigned char * p = v._byteData->data();
		//const unsigned char * p2 = strMessage.c_str();
		//strMessage = boost::make_shared<std::string>((char*)(v._byteData->data()));
		//memcpy(strMessage->c_str(), v._byteData->data(), v._byteData->size());
		std::string sendMsg;
		boost::asio::ip::udp::endpoint targetpoint = v.fromPoint;
		std::string straddress = targetpoint.address().to_string() + ":" + to_string(targetpoint.port());

		_socket.async_send_to(boost::asio::buffer(v._byteData->data(),v._byteData->size()), targetpoint,
			boost::bind(&UDPService::writeHandler, this, strMessage,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

// 		_socket.async_send_to(boost::asio::buffer(*strMessage), targetpoint,
// 			boost::bind(&UDPService::writeHandler,this, strMessage,
// 			boost::asio::placeholders::error,
// 			boost::asio::placeholders::bytes_transferred));
	}
}

void UDPService::readHandler(const system::error_code &ec, size_t bytesTransferred)
{
	if (ec) {
		cout << "udp read error!!" << endl;
		return;
	}
	tagUdpData pNewUdpData;
	pNewUdpData._byteData = boost::make_shared<ByteVector>();
	pNewUdpData._byteData->resize(bytesTransferred, 0);
	uint8_t * p = m_tempBuf.data();
	uint8_t *pdst = (*(pNewUdpData._byteData)).data();
	memcpy(pdst, p, bytesTransferred);
	pNewUdpData.fromPoint = _endpointRemote;
	string str =_endpointRemote.address().to_string();
	int port = _endpointRemote.port();
	if (m_dataRecvCallBackFunc)//有目标端口
		m_dataRecvCallBackFunc(pNewUdpData);
	readOnce();
}

void UDPService::writeHandler(boost::shared_ptr<std::string>, const boost::system::error_code& ec, std::size_t)
{
	if (ec) {
		int a = 0;
	}
	else
	{
		mutex::scoped_lock lock(m_writeQueueMutex);
		m_writeQueue.pop();
		if (m_writeQueue.empty() == false)
		{
			writeOnce();
		}
	}
}

void UDPService::timeoutHandler(const system::error_code &ec)
{
	if (!ec) {
		cout << "Timeout!!" <<endl;
	}
}
