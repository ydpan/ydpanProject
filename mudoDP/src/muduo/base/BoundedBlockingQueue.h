#ifndef MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H


#include <boost/circular_buffer.hpp>
#include <assert.h>
#include <mutex>
#include <condition_variable>

namespace muduo
{

template<typename T>
class BoundedBlockingQueue : noncopyable
{
 public:
  explicit BoundedBlockingQueue(int maxSize)
    : mutex_(),
      queue_(maxSize)
  {
  }

  void put(const T& x)
  {
	std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.full())
    {
      notFull_.wait(lock);
    }
    assert(!queue_.full());
    queue_.push_back(x);
    notEmpty_.notify();
  }

  void put(T&& x)
  {
	std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.full())
    {
      notFull_.wait(lock);
    }
    assert(!queue_.full());
    queue_.push_back(std::move(x));
    notEmpty_.notify_all();
  }

  T take()
  {
	std::unique_lock<std::mutex> lock(mutex_);
	while (queue_.empty())
	{
		notEmpty_.wait(lock);
	}
	assert(!queue_.empty());
	T front(std::move(queue_.front()));
	queue_.pop_front();
	notFull_.notify_all();
	return std::move(front);
  }

  bool empty() const
  {
	  std::unique_lock<std::mutex> lock(mutex_);
      return queue_.empty();
  }

  bool full() const
  {
	  std::unique_lock<std::mutex> lock(mutex_);
      return queue_.full();
  }

  size_t size() const
  {
	  std::unique_lock<std::mutex> lock(mutex_);
      return queue_.size();
  }

  size_t capacity() const
  {
	  std::unique_lock<std::mutex> lock(mutex_);
	  return queue_.capacity();
  }

 private:
  mutable std::mutex          mutex_;
  std::condition_variable     notEmpty_;
  std::condition_variable     notFull_;
  boost::circular_buffer<T>	  queue_ ;
};

}  // namespace muduo

#endif  // MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
