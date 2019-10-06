#include "muduo/base/CountDownLatch.h"
#include <chrono>
using namespace muduo;
/*
它的主要用法是：它允许一个或多个线程等待，直到在其他线程上执行的一组操作完成
*/
CountDownLatch::CountDownLatch(int count)
  : count_(count)
{
}

void muduo::CountDownLatch::wait(long secs /*=0*/)
{
	std::unique_lock<std::mutex> lck(lock_);
	if (0 == count_)
		return;
	if (secs > 0) {
		cv_.wait_for(lck, std::chrono::nanoseconds(secs));
	}
	else {
		cv_.wait(lck);
	}
}

void CountDownLatch::countDown()
{
	std::unique_lock<std::mutex> lck(lock_);
	if (0 == count_)
		return;
	count_--;
	if (0 == count_)
		cv_.notify_all();
}

int CountDownLatch::getCount() 
{
	std::unique_lock<std::mutex> lck(lock_);
	return count_;
}

