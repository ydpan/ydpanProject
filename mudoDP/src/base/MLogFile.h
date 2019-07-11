#ifndef _MLOGINFILE_H_
#define _MLOGINFILE_H_
#include <string>
#include <memory>
#include <mutex>
using namespace std;
namespace mudoDP {
class MLogFile
{
public:
	MLogFile(const string& basename,
		off_t rollSize,
		bool threadStafe = true,
		int flushInterval = 3,
		int checkEveryN = 1024);
	~MLogFile();
	void append(const char* logline, int len);
	void flush();
	bool rollFile();

private:
	void append_unlock(const char* logline, int len);
	static string getLogFileName(const string& basename, time_t* now);
	const string basename_;
	const off_t roolSize_;
	const int flushInterval_;
	const int checkEveryN_;

	int count_;
	std::unique_ptr<mutex> mutex_;
	time_t startOfPeriod_;
	time_t lastRoll_;
	time_t lastFlush_;
	std::unique_ptr<FileUtil::AppendFile> file_;
	const static int kRollPerSeconds_ = 60 * 60 * 24;
};
};
#endif