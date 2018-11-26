#ifndef _SERIALPORTDEVICE_H_
#define _SERIALPORTDEVICE_H_

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <queue>
#include <inttypes.h>
#include "DataGram.h"
using namespace std;
using namespace boost;
using namespace boost::asio;

class SerialPortParams {
public:
	string serialPort{ "/dev/ttyUSB0" };//设备名
	unsigned int baudRate{ 115200 };//波特率
	unsigned int flowControl{ 0 };//流控
	unsigned int parity{ 0 };//校验位
	unsigned int stopBits{ 0 };//停止位
	SerialPortParams()
	{
	}

	SerialPortParams(
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
class SerialPortDevice
{
public:
	SerialPortDevice();
	virtual ~SerialPortDevice();

	void setSerialParams(const SerialPortParams &params);
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

	SerialPortParams	m_serialParams;//设备属性数据
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

#endif // _SERIALPORT_H_
