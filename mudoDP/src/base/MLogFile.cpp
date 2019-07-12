#include "MLogFile.h"

mudoDP::MLogFile::MLogFile(const std::string& basename, off_t rollSize, bool threadStafe /*= true*/, int flushInterval /*= 3*/, int checkEveryN /*= 1024*/):checkEveryN_(checkEveryN), flushInterval_(flushInterval), roolSize_(rollSize)
{

}

mudoDP::MLogFile::~MLogFile()
{

}

void mudoDP::MLogFile::append(const char* logline, int len)
{

}

void mudoDP::MLogFile::flush()
{

}

bool mudoDP::MLogFile::rollFile()
{
	return true;
}

void mudoDP::MLogFile::append_unlock(const char* logline, int len)
{

}

std::string mudoDP::MLogFile::getLogFileName(const string& basename, time_t* now)
{
	return  "";
}
