// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_SINGLETON_H
#define MUDUO_BASE_SINGLETON_H

#include <mutex>
#include <atomic>

namespace muduo
{
	template<typename T>
	class Singleton//µ¥ÀýÄ£Ê½  
	{
	public:
		static T* instance()
		{
			T *sin = s_this.load(std::memory_order_acquire);

			if (!sin) {
				std::lock_guard<std::mutex> locker(s_mutex);
				sin = s_this.load(std::memory_order_relaxed);
				if (!sin) {
					sin = new T;
					s_this.store(sin, std::memory_order_release);
				}
			}

			return sin;
		}

		static void uninstance()
		{
			T *sin = s_this.load(std::memory_order_relaxed);
			if (sin) {
				std::lock_guard<std::mutex> locker(s_mutex);
				delete sin;
				sin = nullptr;
			}
		}

	protected:
		Singleton() = default;
		virtual ~Singleton() = default;
	private:
		Singleton(const T&) = delete;
		T& operator=(const T&) = delete;

		static std::atomic<T*> s_this;
		static std::mutex s_mutex;

	};

	template<class T>
	std::atomic<T*> Singleton<T>::s_this;

	template<class T>
	std::mutex Singleton<T>::s_mutex;
}  // namespace muduo

#endif  // MUDUO_BASE_SINGLETON_H
