#pragma warning(disable : 4996)
#include "EventLoopThread.h"
#include "logging.h"
#include "TcpClient.h"
#include <thread>

using namespace calm;
using namespace calm::net;


int main()
{
	Logger::setLogLevel(Logger::DEBUG);

	EventLoopThread loopThread;
	{
		InetAddress serverAddr("127.0.0.1", 2000); // should succeed
		TcpClient client(loopThread.startLoop(), serverAddr, "TcpClient");
		client.connect();
		std::this_thread::sleep_for(std::chrono::seconds(5));
		client.disconnect();
	}
	std::this_thread::sleep_for(std::chrono::seconds(30));


	return 0;
}
