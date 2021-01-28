#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <intrin.h>
#pragma intrinsic(_subborrow_u32)

#pragma warning(disable:4273 4414)

#define OPTIMIZABLE_C 0

#define MAX_TIME_T     0x0000000793582AFF   // Number of seconds from 00:00:00, 01/01/1970 UTC to 07:59:59, 01/19/3001 UTC
#define MAX_LOCAL_TIME (13 * 60 * 60)       // Maximum local time adjustment (GMT+13 Hours, DST-0 Hours)
#define MIN_LOCAL_TIME (-12 * 60 * 60)      // Minimum local time adjustment (GMT-11 Hours, DST-1 Hours)

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

#ifdef TEST
#include <windows.h>
#include <stdio.h>

static uint64_t __fastcall progress(HANDLE output, DWORD position, uint64_t current)
{
	uint32_t x, y;
	uint64_t next;

	SetConsoleCursorPosition(output, *(COORD *)&position);
	x = (uint32_t)((uint64_t)((current - MIN_LOCAL_TIME) * 10000) / (MAX_TIME_T - MIN_LOCAL_TIME));
	printf("%u.%2.2u%%", x / 100, x % 100);
	next = (uint64_t)((x + 1) * (MAX_TIME_T - MIN_LOCAL_TIME)) / 10000;
	y = (uint32_t)((uint64_t)(next * 10000) / (MAX_TIME_T - MIN_LOCAL_TIME));
	next -= x < y ? -MIN_LOCAL_TIME : -MIN_LOCAL_TIME - 1;
	return next;
}

static BOOL test_gmtime64_s()
{
	#define TEST_HOUR   1
	#define TEST_MINUTE 1
	#define TEST_SECOND 1

	static const uint32_t mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	HANDLE output;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD position;
	uint32_t year, mon, mday, hour, min, sec, days, leap, yday, mdaymax;
	uint64_t current, next;
	struct tm dest;

#if !TEST_HOUR || !TEST_MINUTE || !TEST_SECOND
	srand((unsigned int)time(NULL));
#endif

	GetConsoleScreenBufferInfo(output = GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	next = progress(output, position = *(DWORD *)&csbi.dwCursorPosition, 0);

	days = -1;
	year = 1969;
	leap = !(year % 4) && (year % 100 || !(year % 400));
	yday = 364;
	mon = 11;
	mday = 31;
#if !TEST_HOUR || !TEST_MINUTE || !TEST_SECOND
	hour = 12;
	min = 0;
	sec = 0;

	// first (1969/12/31 12:00:00)
	current = (uint64_t)(int32_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
	_gmtime64_s(&dest, &current);
	if (dest.tm_sec   != sec            ||
		dest.tm_min   != min            ||
		dest.tm_hour  != hour           ||
		dest.tm_mday  != mday           ||
		dest.tm_mon   != mon            ||
		dest.tm_year  != year - 1900    ||
		dest.tm_wday  != (days + 4) % 7 ||
		dest.tm_yday  != yday           ||
		dest.tm_isdst != 0)
		return FALSE;
#endif

	// first day (1969/12/31 12:00:00 - 1969/12/31 23:59:59)
	for (hour = TEST_HOUR ? 12 : rand() % (24 - 12) + 12; hour < 24; hour += TEST_HOUR ? 1 : 24)
	{
		for (min = TEST_MINUTE ? 0 : rand() % 60; min < 60; min += TEST_MINUTE ? 1 : 60)
		{
			for (sec = TEST_SECOND ? 0 : rand() % 60; sec < 60; sec += TEST_SECOND ? 1 : 60)
			{
				current = (uint64_t)(int32_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
				_gmtime64_s(&dest, &current);
				if (dest.tm_sec   != sec            ||
					dest.tm_min   != min            ||
					dest.tm_hour  != hour           ||
					dest.tm_mday  != mday           ||
					dest.tm_mon   != mon            ||
					dest.tm_year  != year - 1900    ||
					dest.tm_wday  != (days + 4) % 7 ||
					dest.tm_yday  != yday           ||
					dest.tm_isdst != 0)
					return FALSE;
				if ((int32_t)current >= (int32_t)next)
					next = progress(output, position, current);
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
				for (hour = TEST_HOUR ? 0 : rand() % 24; hour < 24; hour += TEST_HOUR ? 1 : 24)
				{
					for (min = TEST_MINUTE ? 0 : rand() % 60; min < 60; min += TEST_MINUTE ? 1 : 60)
					{
						for (sec = TEST_SECOND ? 0 : rand() % 60; sec < 60; sec += TEST_SECOND ? 1 : 60)
						{
							current = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
							_gmtime64_s(&dest, &current);
							if (dest.tm_sec   != sec            ||
								dest.tm_min   != min            ||
								dest.tm_hour  != hour           ||
								dest.tm_mday  != mday           ||
								dest.tm_mon   != mon            ||
								dest.tm_year  != year - 1900    ||
								dest.tm_wday  != (days + 4) % 7 ||
								dest.tm_yday  != yday           ||
								dest.tm_isdst != 0)
								return FALSE;
							if (current >= next)
								next = progress(output, position, current);
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
		for (hour = TEST_HOUR ? 0 : rand() % 24; hour < 24; hour += TEST_HOUR ? 1 : 24)
		{
			for (min = TEST_MINUTE ? 0 : rand() % 60; min < 60; min += TEST_MINUTE ? 1 : 60)
			{
				for (sec = TEST_SECOND ? 0 : rand() % 60; sec < 60; sec += TEST_SECOND ? 1 : 60)
				{
					current = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
					_gmtime64_s(&dest, &current);
					if (dest.tm_sec   != sec            ||
						dest.tm_min   != min            ||
						dest.tm_hour  != hour           ||
						dest.tm_mday  != mday           ||
						dest.tm_mon   != mon            ||
						dest.tm_year  != year - 1900    ||
						dest.tm_wday  != (days + 4) % 7 ||
						dest.tm_yday  != yday           ||
						dest.tm_isdst != 0)
						return FALSE;
					if (current >= next)
						next = progress(output, position, current);
				}
			}
		}
		days++;
		yday++;
	}

	// last day (3001/01/19 00:00:00 - 3001/01/19 20:59:59)
	for (hour = TEST_HOUR ? 0 : rand() % 21; hour < 21; hour += TEST_HOUR ? 1 : 24)
	{
		for (min = TEST_MINUTE ? 0 : rand() % 60; min < 60; min += TEST_MINUTE ? 1 : 60)
		{
			for (sec = TEST_SECOND ? 0 : rand() % 60; sec < 60; sec += TEST_SECOND ? 1 : 60)
			{
				current = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
				_gmtime64_s(&dest, &current);
				if (dest.tm_sec   != sec            ||
					dest.tm_min   != min            ||
					dest.tm_hour  != hour           ||
					dest.tm_mday  != mday           ||
					dest.tm_mon   != mon            ||
					dest.tm_year  != year - 1900    ||
					dest.tm_wday  != (days + 4) % 7 ||
					dest.tm_yday  != yday           ||
					dest.tm_isdst != 0)
					return FALSE;
				if (current >= next)
					next = progress(output, position, current);
			}
		}
	}

#if !TEST_HOUR || !TEST_MINUTE || !TEST_SECOND
	// last (3001/01/19 20:59:59)
	hour = 20;
	min = 59;
	sec = 59;
	current = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
	if (current >= next)
	{
		_gmtime64_s(&dest, &current);
		if (dest.tm_sec   != sec            ||
			dest.tm_min   != min            ||
			dest.tm_hour  != hour           ||
			dest.tm_mday  != mday           ||
			dest.tm_mon   != mon            ||
			dest.tm_year  != year - 1900    ||
			dest.tm_wday  != (days + 4) % 7 ||
			dest.tm_yday  != yday           ||
			dest.tm_isdst != 0)
			return FALSE;
		progress(output, position, current);
	}

#endif
	return TRUE;

	#undef TEST_HOUR
	#undef TEST_MINUTE
	#undef TEST_SECOND
}

int main()
{
	printf("testing gmtime64_s by the brute force... ");
	printf(test_gmtime64_s() ? "\nsuccess\n" : "\nfailed\n");
	printf("press any key to continue\n");
	getchar();
	return 0;
}
#endif
