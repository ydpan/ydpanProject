	// Use of this source code is governed by a BSD-style license
	// that can be found in the License file.
	//
	// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_BLOCKINGQUEUE_H
#define MUDUO_BASE_BLOCKINGQUEUE_H

#include <deque>
#include <assert.h>
#include <mutex>
#include <condition_variable>
namespace muduo
{
template<typename T>
class BlockingQueue : noncopyable
{
public:
	BlockingQueue(): mutex_(),queue_()
	{
	}

	void put(const T& x)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push_back(x);
		notEmpty_.notify_all(); // wait morphing saves us
	}

	void put(T&& x)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push_back(std::move(x));
		notEmpty_.notify_all();
	}

	T take()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		// always use a while-loop, due to spurious wakeup
		while (queue_.empty())
		{
			notEmpty_.wait(lock);
		}
		assert(!queue_.empty());
		T front(std::move(queue_.front()));
		queue_.pop_front();
		return std::move(front);
	}

	size_t size() 
	{
		std::unique_lock<std::mutex> lock(mutex_);
		return queue_.size();
	}

private:
	std::mutex					mutex_;
	std::condition_variable		notEmpty_ ;
	std::deque<T>				queue_ ;
	};

}  // namespace muduo

#endif  // MUDUO_BASE_BLOCKINGQUEUE_H
