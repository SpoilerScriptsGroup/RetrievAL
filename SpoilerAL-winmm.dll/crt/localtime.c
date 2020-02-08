#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#pragma warning(disable:4273)

typedef struct {
	int  yr;        /* year of interest */
	int  yd;        /* day of year */
	long ms;        /* milli-seconds in the day */
} transitiondate;

static long                  timezone      = 8 * 3600;
static int                   daylight      = 1;
static long                  dstbias       = -3600;
static char                  tzname[2][64] = { { "PST" }, { "PDT" } };
static int                   tzapiused     = 0;
static TIME_ZONE_INFORMATION tzinfo        = { 0 };
static transitiondate        dststart      = { -1, 0, 0 };
static transitiondate        dstend        = { -1, 0, 0 };

long *__cdecl __timezone()
{
	return &timezone;
}

int *__cdecl __daylight()
{
	return &daylight;
}

long *__cdecl __dstbias()
{
	return &dstbias;
}

char **__cdecl __tzname()
{
	return tzname;
}

void __cdecl _tzset()
{
	size_t count;
	char buffer[64];

	tzapiused = 0;
	dststart.yr = dstend.yr = -1;
	if (getenv_s(&count, buffer, _countof(buffer), "TZ") || !count)
	{
		if (GetTimeZoneInformation(&tzinfo) != TIME_ZONE_ID_INVALID)
		{
			unsigned int defused;

			tzapiused = 1;
			timezone = tzinfo.Bias * 60;
			if (tzinfo.StandardDate.wMonth)
				timezone += (tzinfo.StandardBias * 60);
			dstbias = (daylight = tzinfo.DaylightDate.wMonth && tzinfo.DaylightBias) ?
				(tzinfo.DaylightBias - tzinfo.StandardBias) * 60 : 0;
			if (WideCharToMultiByte(CP_THREAD_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, tzinfo.StandardName, -1, tzname[0], 63, NULL, &defused) && !defused)
				tzname[0][63] = '\0';
			else
				tzname[0][0] = '\0';
			if (WideCharToMultiByte(CP_THREAD_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, tzinfo.DaylightName, -1, tzname[1], 63, NULL, &defused) && !defused)
				tzname[1][63] = '\0';
			else
				tzname[1][0] = '\0';
		}
	}
	else
	{
		char *p, sign;

		if ((tzname[0][0] = *(p = buffer)) &&
			(tzname[0][1] = *(++p)) &&
			(tzname[0][2] = *(++p)))
		{
			tzname[0][3] = '\0';
			p++;
		}
		if ((sign = *p) == '-')
			p++;
		timezone = strtol(p, &p, 10) * 3600;
		if (*p == ':')
		{
			timezone += strtol(p + 1, &p, 10) * 60;
			if (*p == ':')
				timezone += strtol(p + 1, &p, 10);
		}
		if (sign == '-')
			timezone = -timezone;
		if (daylight = tzname[1][0] = *(p++))
			if (tzname[1][1] = *(p++))
				if (tzname[1][2] = *(p++))
					tzname[1][3] = '\0';
	}
}

static void __cdecl cvtdate(
	int trantype,
	int datetype,
	int year,
	int month,
	int week,
	int dayofweek,
	int date,
	int hour,
	int min,
	int sec,
	int msec)
{
	#define DAY_MILLISEC       (1000 * 60 * 60 * 24)
	#define IS_LEAP_YEAR(year) ((year & 3) == 0)
	#define BASE_DOW           4
	#define LEAP_YEAR_ADJUST   17

	const int lpdays[] = { -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	const int days[]   = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };

	int yearday;
	int monthdow;

	if (datetype == 1)
	{
		yearday = 1 + (IS_LEAP_YEAR(year) ? lpdays[month - 1] : days[month - 1]);
		monthdow = (yearday + ((year - 70) * 365) + ((year - 1) >> 2) - LEAP_YEAR_ADJUST + BASE_DOW) % 7;
		yearday += (dayofweek - monthdow) + (week - (monthdow < dayofweek)) * 7;
		if ((week == 5) && (yearday > (IS_LEAP_YEAR(year) ? lpdays[month] : days[month])))
			yearday -= 7;
	}
	else
	{
		yearday = IS_LEAP_YEAR(year) ? lpdays[month - 1] : days[month - 1];
		yearday += date;
	}
	if (trantype == 1)
	{
		dststart.yd = yearday;
		dststart.ms = (long)msec + (1000 * (sec + 60 * (min + 60 * hour)));
		dststart.yr = year;
	}
	else
	{
		dstend.yd = yearday;
		dstend.ms = (long)msec + (1000L * (sec + 60 * (min + 60 * hour)));
		if ((dstend.ms += (dstbias * 1000)) < 0)
		{
			dstend.ms += DAY_MILLISEC;
			dstend.yd--;
		}
		else if (dstend.ms >= DAY_MILLISEC)
		{
			dstend.ms -= DAY_MILLISEC;
			dstend.yd++;
		}
		dstend.yr = year;
	}

	#undef DAY_MILLISEC
	#undef IS_LEAP_YEAR
	#undef BASE_DOW
	#undef LEAP_YEAR_ADJUST
}

static int __cdecl isindst(struct tm *tb)
{
	long ms;

	if (daylight == 0)
		return 0;
	if ((tb->tm_year != dststart.yr) || (tb->tm_year != dstend.yr))
	{
		if (tzapiused)
		{
			if (tzinfo.DaylightDate.wYear == 0)
				cvtdate(1,
					1,             /* day-in-month format */
					tb->tm_year,
					tzinfo.DaylightDate.wMonth,
					tzinfo.DaylightDate.wDay,
					tzinfo.DaylightDate.wDayOfWeek,
					0,
					tzinfo.DaylightDate.wHour,
					tzinfo.DaylightDate.wMinute,
					tzinfo.DaylightDate.wSecond,
					tzinfo.DaylightDate.wMilliseconds);
			else
				cvtdate(1,
					0,             /* absolute date */
					tb->tm_year,
					tzinfo.DaylightDate.wMonth,
					0,
					0,
					tzinfo.DaylightDate.wDay,
					tzinfo.DaylightDate.wHour,
					tzinfo.DaylightDate.wMinute,
					tzinfo.DaylightDate.wSecond,
					tzinfo.DaylightDate.wMilliseconds);
			if (tzinfo.StandardDate.wYear == 0)
				cvtdate(0,
					1,             /* day-in-month format */
					tb->tm_year,
					tzinfo.StandardDate.wMonth,
					tzinfo.StandardDate.wDay,
					tzinfo.StandardDate.wDayOfWeek,
					0,
					tzinfo.StandardDate.wHour,
					tzinfo.StandardDate.wMinute,
					tzinfo.StandardDate.wSecond,
					tzinfo.StandardDate.wMilliseconds);
			else
				cvtdate(0,
					0,             /* absolute date */
					tb->tm_year,
					tzinfo.StandardDate.wMonth,
					0,
					0,
					tzinfo.StandardDate.wDay,
					tzinfo.StandardDate.wHour,
					tzinfo.StandardDate.wMinute,
					tzinfo.StandardDate.wSecond,
					tzinfo.StandardDate.wMilliseconds);

		}
		else
		{
			cvtdate(1,
				1,
				tb->tm_year,
				4,                 /* April */
				1,                 /* first... */
				0,                 /* ...Sunday */
				0,
				2,                 /* 02:00 (2 AM) */
				0,
				0,
				0);
			cvtdate(0,
				1,
				tb->tm_year,
				10,                /* October */
				5,                 /* last... */
				0,                 /* ...Sunday */
				0,
				2,                 /* 02:00 (2 AM) */
				0,
				0,
				0);
		}
	}
	if (dststart.yd < dstend.yd)
	{
		if ((tb->tm_yday < dststart.yd) || (tb->tm_yday > dstend.yd))
			return 0;
		if ((tb->tm_yday > dststart.yd) && (tb->tm_yday < dstend.yd))
			return 1;
	}
	else
	{
		if ((tb->tm_yday < dstend.yd) || (tb->tm_yday > dststart.yd))
			return 1;
		if ((tb->tm_yday > dstend.yd) && (tb->tm_yday < dststart.yd))
			return 0;
	}
	ms = 1000L * (tb->tm_sec + 60L * tb->tm_min + 3600L * tb->tm_hour);
	if (tb->tm_yday == dststart.yd)
		if (ms >= dststart.ms)
			return 1;
		else
			return 0;
	else
		if (ms < dstend.ms)
			return 1;
		else
			return 0;
}

errno_t _localtime32_s(struct tm *dest, const __time32_t *source)
{
	if (dest)
	{
		if (source && *source >= 0)
		{
			#define DAY_SEC (60 * 60 * 24)

			_tzset();
			if ((*source > 3 * DAY_SEC) && (*source < LONG_MAX - 3 * DAY_SEC))
			{
				__time32_t time;

				time = *source - timezone;
				_gmtime32_s(dest, &time);
				if (daylight && isindst(dest))
				{
					time -= dstbias;
					_gmtime32_s(dest, &time);
					dest->tm_isdst = 1;
				}
			}
			else
			{
				long ltime;

				_gmtime32_s(dest, source);
				ltime = (long)dest->tm_sec - timezone + (isindst(dest) ? dstbias : 0);
				dest->tm_sec = (int)(ltime % 60);
				if (dest->tm_sec < 0)
				{
					dest->tm_sec += 60;
					ltime -= 60;
				}
				ltime = (long)dest->tm_min + ltime / 60;
				dest->tm_min = (int)(ltime % 60);
				if (dest->tm_min < 0)
				{
					dest->tm_min += 60;
					ltime -= 60;
				}
				ltime = (long)dest->tm_hour + ltime / 60;
				dest->tm_hour = (int)(ltime % 24);
				if (dest->tm_hour < 0)
				{
					dest->tm_hour += 24;
					ltime -= 24;
				}
				ltime /= 24;
				if (ltime > 0)
				{
					dest->tm_wday = (dest->tm_wday + ltime) % 7;
					dest->tm_mday += ltime;
					dest->tm_yday += ltime;
				}
				else if (ltime < 0)
				{
					dest->tm_wday = (dest->tm_wday + 7 + ltime) % 7;
					if ((dest->tm_mday += ltime) <= 0)
					{
						dest->tm_mday += 31;
						dest->tm_yday = 364;
						dest->tm_mon = 11;
						dest->tm_year--;
					}
					else
					{
						dest->tm_yday += ltime;
					}
				}
			}
			return 0;

			#undef DAY_SEC
		}
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

struct tm* __cdecl _localtime32(__time32_t const* source)
{
	static struct tm dest;
	errno_t error;

	if (!(error = _localtime32_s(&dest, source)))
		return &dest;
	errno = error;
	return NULL;
}

errno_t _localtime64_s(struct tm* dest, const __time64_t* source)
{
	if (dest)
	{
		if (source && *source >= 0)
		{
			#define DAY_SEC (60 * 60 * 24)

			_tzset();
			if ((*source > 3 * DAY_SEC) && (*source < LLONG_MAX - 3 * DAY_SEC))
			{
				__time64_t time;

				time = *source - timezone;
				_gmtime64_s(dest, &time);
				if (daylight && isindst(dest))
				{
					time -= dstbias;
					_gmtime64_s(dest, &time);
					dest->tm_isdst = 1;
				}
			}
			else
			{
				long ltime;

				_gmtime64_s(dest, source);
				ltime = (long)dest->tm_sec - timezone + (isindst(dest) ? dstbias : 0);
				dest->tm_sec = (int)(ltime % 60);
				if (dest->tm_sec < 0)
				{
					dest->tm_sec += 60;
					ltime -= 60;
				}
				ltime = (long)dest->tm_min + ltime / 60;
				dest->tm_min = (int)(ltime % 60);
				if (dest->tm_min < 0)
				{
					dest->tm_min += 60;
					ltime -= 60;
				}
				ltime = (long)dest->tm_hour + ltime / 60;
				dest->tm_hour = (int)(ltime % 24);
				if (dest->tm_hour < 0)
				{
					dest->tm_hour += 24;
					ltime -= 24;
				}
				ltime /= 24;
				if (ltime > 0)
				{
					dest->tm_wday = (dest->tm_wday + ltime) % 7;
					dest->tm_mday += ltime;
					dest->tm_yday += ltime;
				}
				else if (ltime < 0)
				{
					dest->tm_wday = (dest->tm_wday + 7 + ltime) % 7;
					if ((dest->tm_mday += ltime) <= 0)
					{
						dest->tm_mday += 31;
						dest->tm_yday = 364;
						dest->tm_mon = 11;
						dest->tm_year--;
					}
					else
					{
						dest->tm_yday += ltime;
					}
				}
			}
			return 0;

			#undef DAY_SEC
		}
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

struct tm* __cdecl _localtime64(__time64_t const* source)
{
	static struct tm dest;
	errno_t error;

	if (!(error = _localtime64_s(&dest, source)))
		return &dest;
	errno = error;
	return NULL;
}
