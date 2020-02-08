#include <windows.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <intrin.h>
#pragma intrinsic(_subborrow_u32)

errno_t _gmtime32_s(struct tm *dest, const __time32_t *source)
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

errno_t _gmtime64_s(struct tm *dest, const __time64_t *source)
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
