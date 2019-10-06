#include "muduo/base/Thread.h"
#include "muduo/base/CurrentThread.h"
#include "muduo/base/Logging.h"

#include <type_traits>
#include <errno.h>
#include <stdio.h>
#include "Timestamp.h"
#include <chrono>


namespace muduo
{

int gettid()
{
	return CurrentThread::tid();
}

void CurrentThread::cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = gettid();
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

bool CurrentThread::isMainThread()
{
	return CurrentThread::tid() == CurrentThread::pid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(usec));
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const string& n)
  : func_(std::move(func)),name_(n)
{
  setDefaultName();
}

Thread::~Thread()
{
}

void Thread::setDefaultName()
{
  int num = numCreated_.incrementAndGet();
  if (name_.empty())
  {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread%d", num);
    name_ = buf;
  }
}

void Thread::start()
{
  thread_ = std::move(std::thread(func_));
}

int Thread::join()
{
  if(thread_.joinable())
	thread_.join();
  return 0;
}

int Thread::tid()
{
	return CurrentThread::tid();
}

}  // namespace muduo

