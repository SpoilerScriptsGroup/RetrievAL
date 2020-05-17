#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#pragma warning(disable:4273)

#define MAX_TIME_T     0x0000000793582AFF   // Number of seconds from 00:00:00, 01/01/1970 UTC to 07:59:59, 01/19/3001 UTC
#define MAX_LOCAL_TIME (13 * 60 * 60)       // Maximum local time adjustment (GMT+13 Hours, DST-0 Hours)
#define MIN_LOCAL_TIME (-12 * 60 * 60)      // Minimum local time adjustment (GMT-11 Hours, DST-1 Hours)

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

// Sets the time zone information and calculates whether we are currently in
// Daylight Savings Time.  This reads the TZ environment variable, if that
// variable exists and can be read by the process; otherwise, the system is
// queried for the current time zone state.  The daylight, timezone, and
// tzname global varibales are updated accordingly.
int __cdecl _tzset()
{
	#define TZ_STRINGS_SIZE 64

	size_t count;
	char buffer[TZ_STRINGS_SIZE];

	// Clear the flag indicated whether GetTimeZoneInformation was used.
	tz_api_used = 0;

	// Set year fields of dststart and dstend structures to -1 to ensure
	// they are recomputed as after this
	dststart.yr = dstend.yr = -1;

	if (getenv_s(&count, buffer, _countof(buffer), "TZ") || !count)
	{
		if (GetTimeZoneInformation(&tz_info) != TIME_ZONE_ID_INVALID)
		{
			unsigned int used_default_char;
			int status;

			// Record that the API was used:
			tz_api_used = 1;

			// Derive timezone value from Bias and StandardBias fields.
			timezone = (tz_info.Bias + (tz_info.StandardDate.wMonth ? tz_info.StandardBias : 0)) * 60;

			// Check to see if there is a daylight time bias. Since the StandardBias
			// has been added into timezone, it must be compensated for in the
			// value computed for dstbias:
			dstbias = (daylight = tz_info.DaylightDate.wMonth && tz_info.DaylightBias) ?
				(tz_info.DaylightBias - tz_info.StandardBias) * 60 : 0;

			status = WideCharToMultiByte(CP_THREAD_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, tz_info.StandardName, -1, tzname[0], TZ_STRINGS_SIZE - 1, NULL, &used_default_char);
			tzname[0][(status && !used_default_char) ? TZ_STRINGS_SIZE - 1 : 0] = '\0';
			status = WideCharToMultiByte(CP_THREAD_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, tz_info.DaylightName, -1, tzname[1], TZ_STRINGS_SIZE - 1, NULL, &used_default_char);
			tzname[1][(status && !used_default_char) ? TZ_STRINGS_SIZE - 1 : 0] = '\0';
		}
		else
		{
			timezone = 0;
			daylight = 0;
			dstbias  = 0;
		}
	}
	else
	{
		unsigned char *p, sign, c;
		unsigned long maxhour, i, seconds;

		timezone = 0;
		daylight = 0;

		// Process TZ value and update tzname, timezone and daylight.
		if ((tzname[0][0] = *(p = buffer)) &&
			(tzname[0][1] = *(++p)) &&
			(tzname[0][2] = *(++p)))
		{
			tzname[0][3] = '\0';
			p++;
		}

		// The time difference is of the form:
		//     [+|-]hh[:mm[:ss]]
		maxhour = (sign = *p) != '-' ? MAX_LOCAL_TIME / 3600 : -MIN_LOCAL_TIME / 3600;

		// Check for the sign first:
		if (sign == '-' || sign == '+')
			p++;

		// process, then skip over, the hours
		for (i = 0; (unsigned char)(c = *p - '0') < '9' - '0' + 1; p++)
			if ((i = i * 10 + c) > maxhour)
				return 0;
		seconds = i * 3600;

		// Check if minutes were specified:
		if (*p == ':')
		{
			// Process, then skip over, the minutes
			for (i = 0; (unsigned char)(c = *(++p) - '0') < '9' - '0' + 1; )
				if ((i = i * 10 + c) >= 60)
					return 0;
			seconds += i * 60;

			// Check if seconds were specified:
			if (*p == ':')
			{
				// Process, then skip over, the seconds:
				for (i = 0; (unsigned char)(c = *(++p) - '0') < '9' - '0' + 1; )
					if ((i = i * 10 + c) >= 60)
						return 0;
				seconds += i;
			}

			if (seconds > (sign != '-' ? MAX_LOCAL_TIME : (unsigned long)-MIN_LOCAL_TIME))
				return 0;
		}
		timezone = sign != '-' ? seconds : -(long)seconds;

		// Finally, check for a DST zone suffix:
		if (daylight = !!(tzname[1][0] = p[0]))
			if (tzname[1][1] = p[1])
				if (tzname[1][2] = p[2])
					tzname[1][3] = '\0';
	}
	return 1;

	#undef TZ_STRINGS_SIZE
}

// Converts the format of a transition date specification to a value of a
// transitiondate structure.  The dststart and dstend global variables are
// filled in with the converted date.
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
	#define IS_LEAP_YEAR(year)   (!((year) % 4) && ((year) % 100 || !((year) % 400)))
	#define BASE_DOW             4
	#define MILLISECONDS_PER_DAY (24 * 60 * 60 * 1000)

	static const int days[] = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };

	int isleapyear, yearday, monthdow;

	dstbias = 0;
	isleapyear = IS_LEAP_YEAR(year);

	// Figure out the year-day of the start of the month:
	yearday = days[month - 1] + (month - 1 >= 2 && isleapyear);
	if (datetype == DAY_IN_MONTH)
	{
		yearday++;

		// Figureo ut the day of the week of the start of the month:
		monthdow = (unsigned int)(yearday + (year - 70) * 365 + isleapyear + BASE_DOW) % 7;

		// Figure out the year-day of the transition date:
		yearday += dayofweek - monthdow + (week - (monthdow <= dayofweek)) * 7;

		// We may have to adjust the calculation above if week == 5 (meaning the
		// last instance of the day in the month).  Check if the year falls
		// beyond after month and adjust accordingly:
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

		// Set the year field of dststart so that unnecessary calls to cvtdate()
		// may be avoided:
		dststart.yr = year;
	}
	else // END_OF_DST
	{
		dstend.yd = yearday;
		dstend.ms = ((hour * 60 + min) * 60 + sec) * 1000 + msec;

		// The converted date is still a DST date.  We must convert to a standard
		// (local) date while being careful the millisecond field does not
		// overflow or underflow
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

		// Set the year field of dstend so that unnecessary calls to cvtdate()
		// may be avoided:
		dstend.yr = year;
	}

	#undef IS_LEAP_YEAR
	#undef BASE_DOW
	#undef MILLISECONDS_PER_DAY
}

// Implementation Details:  Note that there are two ways that the Daylight
// Savings Time transition data may be returned by GetTimeZoneInformation.  The
// first is a day-in-month format, which is similar to what is used in the USA.
// The transition date is given as the n'th occurrence of a specified day of the
// week in a specified month.  The second is as an absolute date.  The two cases
// are distinguished by the value of the wYear field of the SYSTEMTIME structure
// (zero denotes a day-in-month format).
static bool isindst(struct tm *tb)
{
	long ms;

	if (daylight == 0)
		return 0;

	// Compute (or recompute) the transition dates for Daylight Savings Time
	// if necessary.  The yr fields of dststart and dstend are compared to the
	// year of interest to determine necessity.
	if (tb->tm_year != dststart.yr || tb->tm_year != dstend.yr)
	{
		if (tz_api_used)
		{
			// Convert the start of daylight savings time to dststart:
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

			// Convert start of standard time to dstend:
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

			// The GetTimeZoneInformation API was not used, or failed.  We use
			// the USA Daylight Savings Time rules as a fallback.
			startmonth =  3; // March
			startweek  =  2; // Second week
			endmonth   = 11; // November
			endweek    =  1; // First week

			// The rules changed in 2007:
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

	// Handle simple cases first:
	if (dststart.yd < dstend.yd)
	{
		// Northern hemisphere ordering:
		if (tb->tm_yday < dststart.yd || tb->tm_yday > dstend.yd)
			return false;

		if (tb->tm_yday > dststart.yd && tb->tm_yday < dstend.yd)
			return true;
	}
	else
	{
		// Southern hemisphere ordering:
		if (tb->tm_yday < dstend.yd || tb->tm_yday > dststart.yd)
			return true;

		if (tb->tm_yday > dstend.yd && tb->tm_yday < dststart.yd)
			return false;
	}

	ms = (tb->tm_sec + tb->tm_min * 60 + tb->tm_hour * 3600) * 1000;
	return tb->tm_yday == dststart.yd ?
		ms >= dststart.ms :
		ms < dstend.ms;
}

// The date falls within the first three or last three representable days;
// therefore, it is possible that the time_t representation would overflow
// or underflow while compensating for time zone and daylight savings time.
// Therefore, we make the time zone and daylight savings time adjustments
// directly in the tm structure.
static void correct_first_or_last_three_days(struct tm *dest)
{
	__time32_t ltime;

	ltime = dest->tm_sec;

	// First, adjust for the time zone:
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
		// There is no possibility of overflowing the tm_day and tm_yday
		// members because the date can be no later than January 19.
		dest->tm_wday = (dest->tm_wday + ltime) % 7;
		dest->tm_mday += ltime;
		dest->tm_yday += ltime;
	}
	else if (ltime < 0)
	{
		// It is possible to underflow the tm_mday and tm_yday fields.  If
		// this happens, then the adjusted date must lie in December 1969:
		dest->tm_wday = (dest->tm_wday + 7 + ltime) % 7;
		dest->tm_mday += ltime;
		if (dest->tm_mday <= 0)
		{
			dest->tm_mday += 31;

			// Per assumption #4 above, the time zone can cause the date to
			// underflow the epoch by more than a day.
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
		// Check for illegal time_t value:
		if (source && *source >= 0 && _tzset())
		{
			if ((uint32_t)*source > 3 * DAY_SEC)
			{
				__time32_t ltime;

				// The date does not fall within the first three or last three representable
				// days; therefore, there is no possibility of overflowing or underflowing
				// the time_t representation as we compensate for time zone and daylight
				// savings time.
				ltime = *source - timezone;

				_gmtime32_s(dest, &ltime);

				// Check and adjust for daylight savings time:
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
		// Check for illegal time_t value:
		if (source && *source >= 0 && *source <= MAX_TIME_T && _tzset())
		{
			if (*source > 3 * DAY_SEC && *source <= MAX_TIME_T - 3 * DAY_SEC)
			{
				__time64_t lltime;

				// The date does not fall within the first three or last three representable
				// days; therefore, there is no possibility of overflowing or underflowing
				// the time_t representation as we compensate for time zone and daylight
				// savings time.
				lltime = *source - timezone;

				_gmtime64_s(dest, &lltime);

				// Check and adjust for daylight savings time:
				if (daylight && isindst(dest))
				{
					lltime -= dstbias;
					_gmtime64_s(dest, &lltime);
					dest->tm_isdst = 1;
				}
			}
			else
			{
				_gmtime64_s(dest, source);
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
