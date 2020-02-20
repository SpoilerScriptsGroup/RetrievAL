#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <intrin.h>
#pragma intrinsic(_addcarry_u32)
#pragma intrinsic(_subborrow_u32)

#pragma warning(disable:4273 4414)

#define OPTIMIZABLE_C 0

#define MAX_TIME_T     0x0000000793582AFF   // Number of seconds from 00:00:00, 01/01/1970 UTC to 07:59:59, 01/19/3001 UTC
#define MAX_LOCAL_TIME (13 * 60 * 60)       // Maximum local time adjustment (GMT + 13 Hours, DST -0 Hours)
#define MIN_LOCAL_TIME (-12 * 60 * 60)      // Minimum local time adjustment (GMT - 11 Hours, DST - 1 Hours)

#if !defined(_M_IX86) || OPTIMIZABLE_C
#include "gmtime_c.h"
#else
#include "gmtime_asm.h"
#endif

struct tm * __cdecl _gmtime32(__time32_t const *source)
{
	static struct tm dest;
	errno_t status;

	if (!(status = _gmtime32_s(&dest, source)))
		return &dest;
	errno = status;
	return NULL;
}

struct tm * __cdecl _gmtime64(__time64_t const *source)
{
	static struct tm dest;
	errno_t status;

	if (!(status = _gmtime64_s(&dest, source)))
		return &dest;
	errno = status;
	return NULL;
}

#if TEST
#include <stdlib.h>	// using srand, rand

static uint32_t test_gmtime64_s()
{
	#define TEST_HOUR   1
	#define TEST_MINUTE 1
	#define TEST_SECOND 1

	static const uint32_t mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	uint32_t year, mon, mday, hour, min, sec, days, leap, yday, mdaymax;
	uint64_t source;
	struct tm dest;

#if !TEST_HOUR || !TEST_MINUTE || !TEST_SECOND
	srand((unsigned int)time(NULL));
#endif

	// first day (1969/12/31 12:00:00 - 1969/12/31 23:59:59)
	days = -1;
	year = 1969;
	leap = !(year % 4) && (year % 100 || !(year % 400));
	yday = 364;
	mon = 11;
	mday = 31;
	for (hour = 12; hour < 24; hour++)
	{
		for (min = 0; min < 60; min++)
		{
			for (sec = 0; sec < 60; sec++)
			{
				source = (uint64_t)(int32_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
				_gmtime64_s(&dest, &source);
				if (dest.tm_sec   != sec            ||
					dest.tm_min   != min            ||
					dest.tm_hour  != hour           ||
					dest.tm_mday  != mday           ||
					dest.tm_mon   != mon            ||
					dest.tm_year  != year - 1900    ||
					dest.tm_wday  != (days + 4) % 7 ||
					dest.tm_yday  != yday           ||
					dest.tm_isdst != 0)
					return 0;
			}
		}
	}

	// 1970/01/01 - 3000/12/31
	days = 0;
	for (year = 1970; year < 3001; year++)
	{
		leap = !(year % 4) && (year % 100 || !(year % 400));
		yday = 0;
		for (mon = 0; mon < 12; mon++)
		{
			mdaymax = mdays[mon] + (mon == 1 && leap);
			for (mday = 1; mday <= mdaymax; mday++)
			{
#if TEST_HOUR
				for (hour = 0; hour < 24; hour++)
#else
				hour = rand() % 24;
#endif
				{
#if TEST_MINUTE
					for (min = 0; min < 60; min++)
#else
					min = rand() % 60;
#endif
					{
#if TEST_SECOND
						for (sec = 0; sec < 60; sec++)
#else
						sec = rand() % 60;
#endif
						{
							source = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
							_gmtime64_s(&dest, &source);
							if (dest.tm_sec   != sec            ||
								dest.tm_min   != min            ||
								dest.tm_hour  != hour           ||
								dest.tm_mday  != mday           ||
								dest.tm_mon   != mon            ||
								dest.tm_year  != year - 1900    ||
								dest.tm_wday  != (days + 4) % 7 ||
								dest.tm_yday  != yday           ||
								dest.tm_isdst != 0)
								return 0;
						}
					}
				}
				days++;
				yday++;
			}
		}
	}

	// last month (3001/01/01 - 3001/01/18)
	leap = !(year % 4) && (year % 100 || !(year % 400));
	yday = 0;
	mon = 0;
	for (mday = 1; mday < 19; mday++)
	{
#if TEST_HOUR
		for (hour = 0; hour < 24; hour++)
#else
		hour = rand() % 24;
#endif
		{
#if TEST_MINUTE
			for (min = 0; min < 60; min++)
#else
			min = rand() % 60;
#endif
			{
#if TEST_SECOND
				for (sec = 0; sec < 60; sec++)
#else
				sec = rand() % 60;
#endif
				{
					source = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
					_gmtime64_s(&dest, &source);
					if (dest.tm_sec   != sec            ||
						dest.tm_min   != min            ||
						dest.tm_hour  != hour           ||
						dest.tm_mday  != mday           ||
						dest.tm_mon   != mon            ||
						dest.tm_year  != year - 1900    ||
						dest.tm_wday  != (days + 4) % 7 ||
						dest.tm_yday  != yday           ||
						dest.tm_isdst != 0)
						return 0;
				}
			}
		}
		days++;
		yday++;
	}

	// last day (3001/01/19 00:00:00 - 3001/01/19 20:59:59)
	for (hour = 0; hour < 20; hour++)
	{
		for (min = 0; min < 60; min++)
		{
			for (sec = 0; sec < 60; sec++)
			{
				source = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
				_gmtime64_s(&dest, &source);
				if (dest.tm_sec   != sec            ||
					dest.tm_min   != min            ||
					dest.tm_hour  != hour           ||
					dest.tm_mday  != mday           ||
					dest.tm_mon   != mon            ||
					dest.tm_year  != year - 1900    ||
					dest.tm_wday  != (days + 4) % 7 ||
					dest.tm_yday  != yday           ||
					dest.tm_isdst != 0)
					return 0;
			}
		}
	}
	return 1;

	#undef TEST_HOUR
	#undef TEST_MINUTE
	#undef TEST_SECOND
}

#include <stdio.h>	// using printf, getchar

int main()
{
	printf(test_gmtime64_s() ? "success\n" : "failed\n");
	printf("press any key to continue\n");
	getchar();
	return 0;
}
#endif
