#include "SerialPortDevice.h"
#include <boost/make_shared.hpp>
using namespace std;
using namespace boost;
using namespace boost::asio;

SerialPortDevice::SerialPortDevice()
{
	cout << "SerialPortDevice Object created!" << endl;
	m_tempBuf.resize(1024, 0);
}


SerialPortDevice::~SerialPortDevice()
{
	m_pios->stop();
	m_thread.join();
	cout << "SerialPortDevice Object deleted!" << endl;
}

void SerialPortDevice::setSerialParams(const SerialPortParams &params)
{
	m_serialParams = params;
}

void SerialPortDevice::setTimeOut(int timeout /*= 3000*/)
{
	m_timeOut = timeout;
}

bool SerialPortDevice::startThread()
{
	cout << "SerialPortDevice::statThread() called" << endl;
	m_pios = boost::make_shared<io_service>();
	try {
		// 创建一个serial_port对象, 替换掉原来的
		m_pSerial = boost::make_shared<serial_port>(boost::ref(*m_pios), m_serialParams.serialPort);
		
		m_pSerial->set_option(
			serial_port::baud_rate(m_serialParams.baudRate));
		m_pSerial->set_option(
			serial_port::flow_control((serial_port::flow_control::type)m_serialParams.flowControl));
		m_pSerial->set_option(
			serial_port::parity((serial_port::parity::type)m_serialParams.parity));
		m_pSerial->set_option(
			serial_port::stop_bits((serial_port::stop_bits::type)m_serialParams.stopBits));
		m_pSerial->set_option(
			serial_port::character_size(8));

	}
	catch(std::exception &e){
		cout << "Failed to open serial port !" << endl;
		return false;
	}

	try {
		// 创建线程
		m_thread = boost::thread(boost::bind(&SerialPortDevice::mainRun, this));
	}
	catch (std::exception &e) {
		cout << "Failed to create thread !" << endl;
		return false;
	}

	return true;
}

bool SerialPortDevice::stopThread()
{
	m_pios->stop();
	return true;
}

void SerialPortDevice::setCallbackFunc(const boost::function<void(DataGramPtr)> &func)
{
	m_dataCallbackFunc = func;
}

bool SerialPortDevice::writeDataGram(const DataGram &datagram)
{
	const size_t dataLen = datagram.data.size();
	ByteVector bufToSend(2 + HEADER_LEN + dataLen + 1, 0);
	bufToSend[0] = bufToSend[1] = (uint8_t)0xFF;
	bufToSend[2] = datagram.sender;
	bufToSend[3] = datagram.receiver;
	bufToSend[4] = (uint8_t)(dataLen>>8);
	bufToSend[5] = (uint8_t)(dataLen & 0xFF);
	for(size_t i =0;i<dataLen;i++)
		bufToSend[6+i] = datagram.data.at(i);// TODO

	uint32_t byteSum = 0;
	for (size_t i = 2; i < bufToSend.size() - 1; i++) {
		byteSum += bufToSend.at(i);
	}
	bufToSend[bufToSend.size() - 1] = (uint8_t)(byteSum % 255);
	cout << "sending bytes: " << endl;

	for (size_t i = 0; i < bufToSend.size(); i++)
	{
		//printf("%02X ", bufToSend.at(i));
	}
	cout << endl;
	return writeRaw(bufToSend);
}

bool SerialPortDevice::writeRaw(const ByteVector &rawData)
{
	mutex::scoped_lock lock(m_writeQueueMutex);
	bool writeIdle = m_writeQueue.empty();
	pByteVector data(new ByteVector(rawData));
	m_writeQueue.push(data);
	if (writeIdle)
	{
		start_a_write();
		return true;
	}
	return false;
}

void SerialPortDevice::mainRun()
{
	cout << "SerialPortDevice::mainRun()"<<endl;
	m_state = WAITING_FF1;//初始化工作状态
	m_currentHeader.resize(4, 0);

	start_a_read();
	m_pios->run();
}

void SerialPortDevice::start_a_read()
{
	cout << "SerialPortDevice::start_a_read() " << endl;
	mutex::scoped_lock lock(m_serialMutex);
	m_pSerial->async_read_some(buffer(m_tempBuf), 
		boost::bind(&SerialPortDevice::readHandler,
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void SerialPortDevice::start_a_write()
{
	if (m_pSerial) {
		cout << "SerialPortDevice::start_a_write() " << endl;
		mutex::scoped_lock lock(m_serialMutex);
		async_write(*m_pSerial, buffer(*(m_writeQueue.front())),
			bind(&SerialPortDevice::writeHandler, this, boost::asio::placeholders::error));
	}

	
}

void SerialPortDevice::readHandler(const system::error_code &ec, size_t bytesTransferred)
{
	if (ec) {
		cout << "SerialPortDevice read error!!" << endl;
		return;
	}

	for (size_t i = 0; i < bytesTransferred; i++) {
		uint8_t byte = m_tempBuf.at(i);
		
		switch (m_state) 
		{
		case WAITING_FF1: 
		{
			if (byte == (uint8_t)0xFF) {
				m_state = WAITING_FF2;
				m_ptimer.reset(new deadline_timer(*m_pios,
					posix_time::milliseconds(m_timeOut)));
				m_ptimer->async_wait(bind(&SerialPortDevice::timeoutHandler,
					this, boost::asio::placeholders::error));
			}
			break;
		}
		case WAITING_FF2: 
		{
			if (byte == (uint8_t)0xFF) {
				m_headerBytesRead = 0;
				m_state = READING_HEAD;
			}
			else
				m_state = WAITING_FF1;
			break;
		}
		case READING_HEAD:
		{
			m_currentHeader[m_headerBytesRead] = byte;
			m_headerBytesRead++;
			if (HEADER_LEN == m_headerBytesRead) {
				uint16_t dataLen = ((uint16_t)(m_currentHeader[2]) << 8) + m_currentHeader[3];
				if (dataLen > 0) {
					m_currentData.resize(dataLen, 0);
					m_DataBytesRead = 0;
					m_state = READING_DATA;
				}
				else
					m_state = WAITING_FF1;
			}
			break;
		}
	
		case READING_DATA:
		{
			m_currentData[m_DataBytesRead++] = byte;
			if (m_currentData.size() == m_DataBytesRead) {
				m_state = READING_CHECKSUM;
			}
			break;
		}
		case READING_CHECKSUM:
		{
			m_ptimer->cancel();
			m_ptimer.reset();

			uint32_t byteSum = 0;
			byteSum += 0xFF;
			byteSum += 0xFF;
			for (size_t k = 0; k < m_currentHeader.size(); k++) {
				byteSum += m_currentHeader.at(k);
			}
			for (size_t k = 0; k < m_currentData.size(); k++) {
				byteSum += m_currentData.at(k);
			}

			if ((uint8_t)(byteSum % 255) == byte) {

				cout << "A new datagram received " << endl;
				DataGramPtr pNewDiagram = boost::make_shared<DataGram>();
				pNewDiagram->sender = m_currentHeader.at(0);
				pNewDiagram->receiver = m_currentHeader.at(1);
				pNewDiagram->data = m_currentData;
				m_dataCallbackFunc(pNewDiagram);
			}
			else {
				cout << "checksum error!" << endl;
			}
			m_state = WAITING_FF1;
			break;
		}
		default:
			m_state = WAITING_FF1;
			break;
		}
	}
	start_a_read();
}

void SerialPortDevice::writeHandler(const system::error_code &ec)
{
	if (ec) {

	}
	{
		mutex::scoped_lock lock(m_writeQueueMutex);
		m_writeQueue.pop();
		if (m_writeQueue.empty() == false)
		{
			start_a_write();
		}
	}
}

void SerialPortDevice::timeoutHandler(const system::error_code &ec)
{
	if (!ec) {
		cout << "Timeout!!" <<endl;
		m_state = WAITING_FF1;
	}
}
