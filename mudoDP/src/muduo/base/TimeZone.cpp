// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/base/TimeZone.h"
#include "muduo/base/noncopyable.h"
#include "muduo/base/Date.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
namespace muduo
{
	int wgettimeofday(struct timeval *tv, struct timezone *tz)
	{
		FILETIME ft;
		unsigned __int64 tmpres = 0;
		static int tzflag = 0;

		if (NULL != tv)
		{
			GetSystemTimeAsFileTime(&ft);

			tmpres |= ft.dwHighDateTime;
			tmpres <<= 32;
			tmpres |= ft.dwLowDateTime;

			tmpres /= 10;  /*convert into microseconds*/
			/*converting file time to unix epoch*/
			tmpres -= DELTA_EPOCH_IN_MICROSECS;
			tv->tv_sec = (long)(tmpres / 1000000UL);
			tv->tv_usec = (long)(tmpres % 1000000UL);
		}

		if (NULL != tz)
		{
			if (!tzflag)
			{
				_tzset();
				tzflag++;
			}
			tz->tz_minuteswest = _timezone / 60;
			tz->tz_dsttime = _daylight;
		}

		return 0;
	}

	int wgmtime(struct timeval *tv, SYSTEMTIME *st)
	{
		FILETIME ft;
		unsigned __int64 tmpres = 0;
		tmpres = ((unsigned __int64)tv->tv_sec * 1000000UL) + (unsigned __int64)tv->tv_usec;
		tmpres += DELTA_EPOCH_IN_MICROSECS;
		tmpres *= 10;

		ft.dwLowDateTime = tmpres & 0xFFFFFFFF;
		ft.dwHighDateTime = (tmpres >> 32) & 0xFFFFFFFF;
		FileTimeToSystemTime(&ft, st);
		return 0;
	}
}
