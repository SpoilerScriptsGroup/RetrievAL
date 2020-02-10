#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <intrin.h>
#pragma intrinsic(_subborrow_u32)

errno_t __cdecl _gmtime32_s(struct tm *dest, const __time32_t *source)
{
	if (dest)
	{
		uint32_t time;

		if (source && (int32_t)(time = *source) >= 0)
			#include "gmtime_common.h"
		dest->tm_sec   = -1;
		dest->tm_min   = -1;
		dest->tm_hour  = -1;
		dest->tm_mday  = -1;
		dest->tm_mon   = -1;
		dest->tm_year  = -1;
		dest->tm_wday  = -1;
		dest->tm_yday  = -1;
		dest->tm_isdst = -1;
	}
	return EINVAL;
}

struct tm *__cdecl _gmtime32(__time32_t const *source)
{
	static struct tm dest;
	errno_t error;

	if (!(error = _gmtime32_s(&dest, source)))
		return &dest;
	errno = error;
	return NULL;
}

errno_t __cdecl _gmtime64_s(struct tm *dest, const __time64_t *source)
{
	if (dest)
	{
		uint64_t time;

		if (source && (int64_t)(time = *source) >= 0)
			if (time <= INT32_MAX)
				return _gmtime32_s(dest, (const __time32_t *)source);
			else
				#include "gmtime_common.h"
		dest->tm_sec   = -1;
		dest->tm_min   = -1;
		dest->tm_hour  = -1;
		dest->tm_mday  = -1;
		dest->tm_mon   = -1;
		dest->tm_year  = -1;
		dest->tm_wday  = -1;
		dest->tm_yday  = -1;
		dest->tm_isdst = -1;
	}
	return EINVAL;
}

struct tm *__cdecl _gmtime64(__time64_t const *source)
{
	static struct tm dest;
	errno_t error;

	if (!(error = _gmtime64_s(&dest, source)))
		return &dest;
	errno = error;
	return NULL;
}

#if TEST
#include <stdlib.h>	// using srand, rand
static uint32_t test_gmtime64_s()
{
	static const uint32_t mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	uint32_t year, mon, mday, hour, min, sec, days, leap, yday, mdaymax;
	uint64_t source;
	struct tm dest;

	srand((unsigned int)time(NULL));
	days = 0;
	for (year = 1970; year <= 10000; year++)
	{
		leap = !(year % 4) && (year % 100 || !(year % 400));
		yday = 0;
		for (mon = 0; mon < 12; mon++)
		{
			mdaymax = mdays[mon] + (mon == 1 && leap);
			for (mday = 1; mday <= mdaymax; mday++)
			{
				hour = rand() % 24;
				min  = rand() % 60;
				sec  = rand() % 60;
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
				days++;
				yday++;
			}
		}
	}
	return 1;
}

int main()
{
	printf(test_gmtime64_s() ? "success\n" : "failed\n");
	return 0;
}
#endif
