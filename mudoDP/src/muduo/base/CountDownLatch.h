// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H
#include <mutex>
#include <condition_variable>

namespace muduo
{

class CountDownLatch
{
 public:

  explicit CountDownLatch(int count);

  void wait(long secs =0);

  void countDown();

  int getCount();

 private:
	 std::condition_variable cv_;
	 std::mutex lock_;
	 unsigned int count_;

	 //delete constructors /assignment operators
	 CountDownLatch() = delete;
	 CountDownLatch(const CountDownLatch& other) = delete;
	 CountDownLatch& operator=(const CountDownLatch& opther) = delete;

};

}  // namespace muduo
#endif  // MUDUO_BASE_COUNTDOWNLATCH_H
