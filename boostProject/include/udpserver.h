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
typedef struct tagUdpData{// �����������ݵ�Դ�˿ڼ�����
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
		ͨ�����ַ�ʽ���� �󶨻ص�����
	setCallbackFunc(boost::bind(&boostSeriport::serialCallback,this,_1));
	*/
	void setCallbackFunc(const boost::function<void(tagUdpData)> &func);

	bool writeRaw(const ByteVector &rawData);
	bool writeRaw(const tagUdpData &tagData);
	bool writeRaw(const boost::asio::ip::udp::endpoint _endpoint, const ByteVector &rawData);
private:
	boost::shared_ptr<deadline_timer> m_ptimer;//��ʱ��ʱ��
	io_service m_pios;//io_service 
	mutex m_udpMutex;//������ 

	int					m_timeOut{ 1000 };//��ʱʱ��

	int					m_bufSize{ 1024 };//Ĭ�ϻ�������С
	ByteVector			m_tempBuf;//���ݶ�ȡ������
	
	queue<tagUdpData>	m_writeQueue;//���������ݶ���
	mutex				m_writeQueueMutex;//���л�����

	boost::function<void(tagUdpData)> m_dataRecvCallBackFunc;//���ݻص�����
	boost::function<void()> m_errorCallbackFunc;//����ص�����

	boost::thread m_thread;//io_service::run�߳���

	void mainRun();

	void readOnce();//��һ��
	void writeOnce();//дһ��

	void readHandler(const system::error_code &ec, size_t bytesTransferred);
	void writeHandler(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t);
	void timeoutHandler(const system::error_code &ec);


	udp::socket _socket;
	udp::endpoint _endpointRemote;//Զ�̷������ݵĶ˵�  ��������ʱ��ȡ IP���˿ں�
};

#endif // _SERIALPORT_H_
