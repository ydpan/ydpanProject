// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_TIMEZONE_H
#define MUDUO_BASE_TIMEZONE_H

#include "muduo/base/copyable.h"
#include <memory>
#include <time.h>
#include <windows.h>
namespace muduo
{

	struct timezone
	{
		int  tz_minuteswest; /* minutes W of Greenwich */
		int  tz_dsttime;     /* type of dst correction */
	};
	int wgettimeofday(struct timeval *tv, struct timezone *tz);
	int wgmtime(struct timeval *tv, SYSTEMTIME *st);

}  // namespace muduo

#endif  // MUDUO_BASE_TIMEZONE_H
