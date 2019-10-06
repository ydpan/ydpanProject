#include "muduo/base/LogFile.h"
#include "muduo/base/Logging.h"
#include "muduo/base/CurrentThread.h"
//2019-09-30 bob
std::unique_ptr<muduo::LogFile> g_logFile;

void outputFunc(const char* msg, int len)
{
	g_logFile->append(msg, len);
}

void flushFunc()
{
	g_logFile->flush();
}

int main(int argc, char* argv[])
{
	char name[256] = { 0 };
	strncpy(name, argv[0], sizeof name - 1);
	g_logFile.reset(new muduo::LogFile("test", 200 * 1000));
	muduo::Logger::setOutput(outputFunc);
	muduo::Logger::setFlush(flushFunc);

	muduo::string line = "qweqwrqwr qw  abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

	for (int i = 0; i < 10000; ++i)
	{
		LOG_INFO << line << i;

		muduo::CurrentThread::sleepUsec(1000);
	}
}
