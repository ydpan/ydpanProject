// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include "muduo/base/Types.h"
#include <thread>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>
#include <process.h>

namespace muduo
{
namespace CurrentThread
{
  // internal
  extern  int t_cachedTid;
  extern  char t_tidString[32];
  extern  int t_tidStringLength;
  extern  const char* t_threadName;
  void cacheTid();

  inline int tid()
  {
	  std::stringstream sin;
	  sin << std::this_thread::get_id();
	  int threadID = std::stoull(sin.str());
	  return threadID;
  }
  inline int pid()
  {
	  return getpid();
  }
  inline const char* tidString() // for logging
  {
    return t_tidString;
  }
  inline const uint64_t convertThreadid(std::thread::id pid_t)
  {
	  std::stringstream ss;
	  ss << pid_t;
	  return std::stoull(ss.str());
  }
  inline int tidStringLength() // for logging
  {
    return t_tidStringLength;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  bool isMainThread();

  void sleepUsec(int64_t usec);  // for testing

  string stackTrace(bool demangle);
}  // namespace CurrentThread
}  // namespace muduo

#endif  // MUDUO_BASE_CURRENTTHREAD_H
