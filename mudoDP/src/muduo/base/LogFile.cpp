#include "LogFile.h"

#include <assert.h>
#include <iostream>
#include <fstream>

using namespace muduo;
// not thread safe
class LogFile::File : boost::noncopyable
{
 public:
  explicit File(const string& filename):
      writtenBytes_(0)
  {
	  _file.open(filename.data(), std::ios::out | std::ios::trunc);
  }

  ~File()
  {
	  _file.close();
  }

  void append(const char* logline, const size_t len)
  {
    size_t n = write(logline, len);
    size_t remain = len - n;
    while (remain > 0)
    {
      size_t x = write(logline + n, remain);
      if (x == 0)
      {
		int err = 0;// ferror(fp_);
        if (err)
        {
          char buf[128];
         // strerror_r(err, buf, sizeof buf); // FIXME: strerror_l
          fprintf(stderr, "LogFile::File::append() failed %s\n", buf);
        }
        break;
      }
      n += x;
      remain = len - n; // remain -= x
    }

    writtenBytes_ += len;
  }

  void flush()
  {
	  _file.flush();
  }

  size_t writtenBytes() const { return writtenBytes_; }

 private:

  size_t write(const char* logline, size_t len)
  {
	  if (_file)
	  {
		  _file.write(logline, len);
		  return len;
	  }
	  return 0;
  }

  std::ofstream _file;
  char buffer_[64*1024];
  size_t writtenBytes_;
};

LogFile::LogFile(const string& basename,
                 size_t rollSize,
                 bool threadSafe,
                 int flushInterval)
  : basename_(basename),
    rollSize_(rollSize),
    flushInterval_(flushInterval),
    count_(0),
    mutex_(threadSafe ? new std::mutex : NULL),
    startOfPeriod_(0),
    lastRoll_(0),
    lastFlush_(0)
{
  assert(basename.find('/') == string::npos);
  rollFile();
}

LogFile::~LogFile()
{
}

void LogFile::append(const char* logline, int len)
{
  if (mutex_)
  {
    std::lock_guard<std::mutex> guard(*mutex_);
    append_unlocked(logline, len);
  }
  else
  {
    append_unlocked(logline, len);
  }
}

void LogFile::flush()
{
  if (mutex_)
  {
	std::lock_guard<std::mutex> guard(*mutex_);
    file_->flush();
  }
  else
  {
    file_->flush();
  }
}

void LogFile::append_unlocked(const char* logline, int len)
{
  file_->append(logline, len);

  if (file_->writtenBytes() > rollSize_)
  {
    rollFile();
  }
  else
  {
    if (count_ > kCheckTimeRoll_)
    {
      count_ = 0;
	  time_t now = 0;
	  time(&now);
      time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
      if (thisPeriod_ != startOfPeriod_)
      {
        rollFile();
      }
      else if (now - lastFlush_ > flushInterval_)
      {
        lastFlush_ = now;
        file_->flush();
      }
    }
    else
    {
      ++count_;
    }
  }
}

void LogFile::rollFile()
{
  time_t now = 0;
  string filename = getLogFileName(basename_, &now);
  time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

  if (now > lastRoll_||!file_.get())
  {
    lastRoll_ = now;
    lastFlush_ = now;
    startOfPeriod_ = start;
    file_.reset(new File(filename));
  }
}

string LogFile::getLogFileName(const string& basename, time_t* now)
{
  string filename;
  filename.reserve(basename.size() + 32);
  filename = basename;

  char timebuf[32];
  char pidbuf[32];
  struct tm *tm;
  time(now);
  tm = localtime(now);
  strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S", tm);
  filename += timebuf;
  snprintf(pidbuf, sizeof pidbuf, ".%d", std::this_thread::get_id()); 
  filename += pidbuf;
  filename += ".log";

  return filename;
}

