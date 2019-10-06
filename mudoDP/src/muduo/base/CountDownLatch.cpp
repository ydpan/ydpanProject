#include "muduo/base/CountDownLatch.h"
#include <chrono>
using namespace muduo;
/*
������Ҫ�÷��ǣ�������һ�������̵߳ȴ���ֱ���������߳���ִ�е�һ��������
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

