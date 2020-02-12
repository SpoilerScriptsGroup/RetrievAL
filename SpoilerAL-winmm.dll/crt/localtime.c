#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#pragma warning(disable:4273)

// Number of seconds from 00:00:00, 01/01/1970 UTC to 23:59:59, 01/18/3001 UTC+8
#define MAX_TIME_T 0x0000000793582AFF

// Maximum local time adjustment (GMT + 13 Hours, DST -0 Hours)
#define MAX_LOCAL_TIME (13 * 60 * 60)

// Minimum local time adjustment (GMT - 11 Hours, DST - 1 Hours)
#define MIN_LOCAL_TIME (-12 * 60 * 60)

// Structure used to represent DST transition date/times:
typedef struct {
	int  yr; // year of interest
	int  yd; // day of year
	int  ms; // milli-seconds in the day
} transitiondate;

// date type
#define ABSOLUTE_DATE 0
#define DAY_IN_MONTH  1

// transition type
#define START_OF_DST  0
#define END_OF_DST    1

static long                  timezone      = 8 * 3600;
static int                   daylight      = 1;
static long                  dstbias       = -3600;
static char                  tzname[2][64] = { { "PST" }, { "PDT" } };
static int                   tz_api_used   = 0;
static TIME_ZONE_INFORMATION tz_info       = { 0 };
static transitiondate        dststart      = { -1, 0, 0 };
static transitiondate        dstend        = { -1, 0, 0 };

long * __cdecl __timezone()
{
	return &timezone;
}

int * __cdecl __daylight()
{
	return &daylight;
}

long * __cdecl __dstbias()
{
	return &dstbias;
}

char ** __cdecl __tzname()
{
	return (char **)tzname;
}

void __cdecl _tzset()
{
	#define TZ_STRINGS_SIZE 64

	size_t count;
	char buffer[TZ_STRINGS_SIZE];

	tz_api_used = 0;
	dststart.yr = dstend.yr = -1;
	if (getenv_s(&count, buffer, _countof(buffer), "TZ") || !count)
	{
		if (GetTimeZoneInformation(&tz_info) != TIME_ZONE_ID_INVALID)
		{
			unsigned int used_default_char;

			tz_api_used = 1;
			timezone = tz_info.Bias * 60;
			if (tz_info.StandardDate.wMonth)
				timezone += (tz_info.StandardBias * 60);
			dstbias = (daylight = tz_info.DaylightDate.wMonth && tz_info.DaylightBias) ?
				(tz_info.DaylightBias - tz_info.StandardBias) * 60 : 0;
			tzname[0][
				(WideCharToMultiByte(CP_THREAD_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, tz_info.StandardName, -1, tzname[0], TZ_STRINGS_SIZE - 1, NULL, &used_default_char) && !used_default_char) ?
					TZ_STRINGS_SIZE - 1 : 0] = '\0';
			tzname[1][
				(WideCharToMultiByte(CP_THREAD_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, tz_info.DaylightName, -1, tzname[1], TZ_STRINGS_SIZE - 1, NULL, &used_default_char) && !used_default_char) ?
					TZ_STRINGS_SIZE - 1 : 0] = '\0';
		}
	}
	else
	{
		unsigned char* p, c;
		unsigned int nagate, i, j;

		if ((tzname[0][0] = *(p = buffer)) &&
			(tzname[0][1] = *(++p)) &&
			(tzname[0][2] = *(++p)))
		{
			tzname[0][3] = '\0';
			p++;
		}
		if ((nagate = (c = *p) == '-') || c == '+')
			p++;
		for (i = 0; (unsigned char)(c = *p - '0') <= '9' - '0'; p++)
		{
			if ((i = i * 10 + c) > 13 - nagate)
			{
				i = 13 - nagate;
				while ((unsigned char)(*p - '0') <= '9' - '0')
					p++;
				break;
			}
		}
		j = i * 3600;
		if (*p == ':')
		{
			for (i = 0; (unsigned char)(c = *(++p) - '0') <= '9' - '0'; )
			{
				if ((i = i * 10 + c) > 59)
				{
					i = 59;
					while ((unsigned char)(*(++p) - '0') <= '9' - '0');
					break;
				}
			}
			j += i * 60;
			if (*p == ':')
			{
				for (i = 0; (unsigned char)(c = *(++p) - '0') <= '9' - '0'; )
				{
					if ((i = i * 10 + c) > 59)
					{
						i = 59;
						while ((unsigned char)(*(++p) - '0') <= '9' - '0');
						break;
					}
				}
				j += i;
			}
		}
		timezone = !nagate ? min(j, MAX_LOCAL_TIME) : -(long)min(j, -MIN_LOCAL_TIME);
		if (daylight = !!(tzname[1][0] = p[0]))
			if (tzname[1][1] = p[1])
				if (tzname[1][2] = p[2])
					tzname[1][3] = '\0';
	}

	#undef TZ_STRINGS_SIZE
}

static void cvtdate(
	int const trantype,  // start or end of DST
	int const datetype,  // Day-in-month or absolute date
	int const year,      // Year, as an offset from 1900
	int const month,     // Month, where 0 is January
	int const week,      // Week of month, if datetype is day-in-month
	int const dayofweek, // Day of week, if datetype is day-in-month
	int const date,      // Date of month (1 - 31)
	int const hour,      // Hours (0 - 23)
	int const min,       // Minutes (0 - 59)
	int const sec,       // Seconds (0 - 59)
	int const msec)      // Milliseconds (0 - 999)
{
	#define IS_LEAP_YEAR(year)   ((year & 3) == 0)
	#define BASE_DOW             4
	#define MILLISECONDS_PER_DAY (24 * 60 * 60 * 1000)

	const int days[] = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };

	int yearday, monthdow, isleapyear;

	dstbias = 0;
	isleapyear = IS_LEAP_YEAR(year);
	yearday = days[month - 1] + (month - 1 >= 2 && isleapyear);
	if (datetype == DAY_IN_MONTH)
	{
		yearday++;
		monthdow = (unsigned int)(yearday + (year - 70) * 365 + isleapyear + BASE_DOW) % 7;
		yearday += dayofweek - monthdow + (week - (monthdow <= dayofweek)) * 7;
		if (week == 5 && yearday > days[month] + (month >= 2 && isleapyear))
			yearday -= 7;
	}
	else
	{
		yearday += date;
	}
	if (trantype == START_OF_DST)
	{
		dststart.yd = yearday;
		dststart.ms = ((hour * 60 + min) * 60 + sec) * 1000 + msec;
		dststart.yr = year;
	}
	else // END_OF_DST
	{
		dstend.yd = yearday;
		dstend.ms = ((hour * 60 + min) * 60 + sec) * 1000 + msec;
		dstend.ms += dstbias * 1000;
		if (dstend.ms < 0)
		{
			dstend.ms += MILLISECONDS_PER_DAY;
			dstend.yd--;
		}
		else if (dstend.ms >= MILLISECONDS_PER_DAY)
		{
			dstend.ms -= MILLISECONDS_PER_DAY;
			dstend.yd++;
		}
		dstend.yr = year;
	}

	#undef IS_LEAP_YEAR
	#undef BASE_DOW
	#undef MILLISECONDS_PER_DAY
}

static bool isindst(struct tm *tb)
{
	long ms;

	if (daylight == 0)
		return 0;
	if (tb->tm_year != dststart.yr || tb->tm_year != dstend.yr)
	{
		if (tz_api_used)
		{
			if (tz_info.DaylightDate.wYear == 0)
			{
				cvtdate(
					START_OF_DST,
					DAY_IN_MONTH,
					tb->tm_year,
					tz_info.DaylightDate.wMonth,
					tz_info.DaylightDate.wDay,
					tz_info.DaylightDate.wDayOfWeek,
					0,
					tz_info.DaylightDate.wHour,
					tz_info.DaylightDate.wMinute,
					tz_info.DaylightDate.wSecond,
					tz_info.DaylightDate.wMilliseconds);
			}
			else
			{
				cvtdate(
					START_OF_DST,
					ABSOLUTE_DATE,
					tb->tm_year,
					tz_info.DaylightDate.wMonth,
					0,
					0,
					tz_info.DaylightDate.wDay,
					tz_info.DaylightDate.wHour,
					tz_info.DaylightDate.wMinute,
					tz_info.DaylightDate.wSecond,
					tz_info.DaylightDate.wMilliseconds);
			}
			if (tz_info.StandardDate.wYear == 0)
			{
				cvtdate(
					END_OF_DST,
					DAY_IN_MONTH,
					tb->tm_year,
					tz_info.StandardDate.wMonth,
					tz_info.StandardDate.wDay,
					tz_info.StandardDate.wDayOfWeek,
					0,
					tz_info.StandardDate.wHour,
					tz_info.StandardDate.wMinute,
					tz_info.StandardDate.wSecond,
					tz_info.StandardDate.wMilliseconds);
			}
			else
			{
				cvtdate(
					END_OF_DST,
					ABSOLUTE_DATE,
					tb->tm_year,
					tz_info.StandardDate.wMonth,
					0,
					0,
					tz_info.StandardDate.wDay,
					tz_info.StandardDate.wHour,
					tz_info.StandardDate.wMinute,
					tz_info.StandardDate.wSecond,
					tz_info.StandardDate.wMilliseconds);
			}
		}
		else
		{
			int startmonth, startweek, endmonth, endweek;

			startmonth =  3; // March
			startweek  =  2; // Second week
			endmonth   = 11; // November
			endweek    =  1; // First week
			if (107 > tb->tm_year)
			{
				startmonth =  4; // April
				startweek  =  1; // first week
				endmonth   = 10; // October
				endweek    =  5; // last week
			}
			cvtdate(
				START_OF_DST,
				DAY_IN_MONTH,
				tb->tm_year,
				startmonth,
				startweek,
				0, // Sunday
				0,
				2, // 02:00 (2 AM)
				0,
				0,
				0);
			cvtdate(
				END_OF_DST,
				DAY_IN_MONTH,
				tb->tm_year,
				endmonth,
				endweek,
				0, // Sunday
				0,
				2, // 02:00 (2 AM)
				0,
				0,
				0);
		}
	}
	if (dststart.yd < dstend.yd)
	{
		if (tb->tm_yday < dststart.yd || tb->tm_yday > dstend.yd)
			return false;
		if (tb->tm_yday > dststart.yd && tb->tm_yday < dstend.yd)
			return true;
	}
	else
	{
		if (tb->tm_yday < dstend.yd || tb->tm_yday > dststart.yd)
			return true;
		if (tb->tm_yday > dstend.yd && tb->tm_yday < dststart.yd)
			return false;
	}
	ms = (tb->tm_sec + tb->tm_min * 60 + tb->tm_hour * 3600) * 1000;
	if (tb->tm_yday == dststart.yd)
		return ms >= dststart.ms;
	else
		return ms < dstend.ms;
}

static void correct_first_or_last_three_days(struct tm *dest)
{
	__time32_t ltime;

	ltime = dest->tm_sec;
	if (daylight && isindst(dest))
	{
		ltime -= timezone + dstbias;
		dest->tm_isdst = 1;
	}
	else
	{
		ltime -= timezone;
	}
	dest->tm_sec = ltime % 60;
	if (dest->tm_sec < 0)
	{
		dest->tm_sec += 60;
		ltime -= 60;
	}
	ltime = dest->tm_min + ltime / 60;
	dest->tm_min = ltime % 60;
	if (dest->tm_min < 0)
	{
		dest->tm_min += 60;
		ltime -= 60;
	}
	ltime = dest->tm_hour + ltime / 60;
	dest->tm_hour = ltime % 24;
	if (dest->tm_hour < 0)
	{
		dest->tm_hour += 24;
		ltime -=24;
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
		dest->tm_mday += ltime;
		if (dest->tm_mday <= 0)
		{
			dest->tm_mday += 31;
			dest->tm_yday = dest->tm_yday + ltime + 365;
			dest->tm_mon = 11;
			dest->tm_year--;
		}
		else
		{
			dest->tm_yday += ltime;
		}
	}
}

errno_t __cdecl _localtime32_s(struct tm *dest, const __time32_t *source)
{
	#define DAY_SEC (60 * 60 * 24)

	if (dest)
	{
		if (source)
		{
			_tzset();
			if ((uint32_t)*source > 3 * DAY_SEC)
			{
				__time32_t ltime;

				ltime = *source - timezone;
				_gmtime32_s(dest, &ltime);
				if (daylight && isindst(dest))
				{
					ltime -= dstbias;
					_gmtime32_s(dest, &ltime);
					dest->tm_isdst = 1;
				}
			}
			else
			{
				_gmtime32_s(dest, source);
				correct_first_or_last_three_days(dest);
			}
			return 0;
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

	#undef DAY_SEC
}

struct tm * __cdecl _localtime32(__time32_t const *source)
{
	static struct tm dest;
	errno_t status;

	if (!(status = _localtime32_s(&dest, source)))
		return &dest;
	errno = status;
	return NULL;
}

errno_t __cdecl _localtime64_s(struct tm *dest, const __time64_t *source)
{
	#define DAY_SEC (60 * 60 * 24)

	if (dest)
	{
		if (source)
		{
			errno_t status;

			_tzset();
			if (*source > 3 * DAY_SEC && *source <= MAX_TIME_T - 3 * DAY_SEC)
			{
				__time64_t lltime;

				lltime = *source - timezone;
				if (status = _gmtime64_s(dest, &lltime))
					return status;
				if (daylight && isindst(dest))
				{
					lltime -= dstbias;
					if (status = _gmtime64_s(dest, &lltime))
						return status;
					dest->tm_isdst = 1;
				}
			}
			else
			{
				if (status = _gmtime64_s(dest, source))
					return status;
				correct_first_or_last_three_days(dest);
			}
			return 0;
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

	#undef DAY_SEC
}

struct tm * __cdecl _localtime64(__time64_t const *source)
{
	static struct tm dest;
	errno_t status;

	if (!(status = _localtime64_s(&dest, source)))
		return &dest;
	errno = status;
	return NULL;
}
