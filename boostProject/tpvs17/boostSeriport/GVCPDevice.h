#ifndef _GVCP_DEVICE_H_
#define _GVCP_DEVICE_H_
#include <string.h>
#include <vector>
#include <boost/asio.hpp>
#include "udpserver.h"
#include "DeviceInfo.h"
#include "gvcp.h"

using namespace boost;

class GVCPDevice
{
public:
	GVCPDevice();
	~GVCPDevice();
	void InitDevice();
	void readDeViceInfo();

	void handMsgCallBack(tagUdpData tagData);
private:
	void DiscoveryAck(tagUdpData &tagdata);
	void ReadRegisterAck(tagUdpData &tagdata);
	void WriteRegisterAck(tagUdpData &tagdata);
	void ReadMemoryAck(tagUdpData &tagdata);
	void WriteMemoryAck(tagUdpData &tagdata);
	void FocusIPAck(tagUdpData &tagdata);
private:
	boost::shared_ptr<UDPService> m_pUdpServer;
	boost::asio::ip::udp::endpoint _fromPoint;
	char                 _cRecvData[ARV_GVCP_MAX_MSG_LEN];
	unsigned int         _nLastAckId{0};

	DeviceInfo m_device;
};

#endif
