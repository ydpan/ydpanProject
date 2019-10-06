#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include "muduo/base/Atomic.h"
#include "muduo/base/CountDownLatch.h"
#include "muduo/base/Types.h"
#include <functional>
#include <memory>

namespace muduo
{

	class MyThread :public std::thread
	{
	public:
		MyThread() :thread()
		{

		}
		template<typename T, typename...Args>
		MyThread(T&&func, Args&&...args) : thread(forward<T>(func), forward<Args>(args)...)
		{

		}
		void showcmd(const char *str) {

		}
	};
	class Thread/* : noncopyable*/
	{
	public:
	typedef std::function<void ()> ThreadFunc;

	explicit Thread(ThreadFunc, const string& name = string());
	// FIXME: make it movable in C++11
	~Thread();

	void start();
	int join(); // return pthread_join()

	int tid();
	const string& name() const { return name_; }

	static int numCreated() { return numCreated_.get(); }

	private:
	void setDefaultName();
	ThreadFunc func_;
	string     name_;
	std::thread thread_;
	static AtomicInt32 numCreated_;
	};

}  // namespace muduo
#endif  // MUDUO_BASE_THREAD_H
