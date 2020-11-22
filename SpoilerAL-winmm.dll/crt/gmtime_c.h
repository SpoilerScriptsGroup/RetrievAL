static errno_t internal_gmtime32(struct tm *dest, uint32_t time32);
static errno_t internal_gmtime64(struct tm *dest, uint64_t time64);
static errno_t internal_gmtime_less_than_400_years_left(struct tm *dest, uint32_t year, uint32_t days);

errno_t __cdecl _gmtime32_s(struct tm *dest, const __time32_t *source)
{
	if (dest)
	{
		uint32_t time32;

		if (source && (int32_t)(time32 = *source) >= MIN_LOCAL_TIME)
			return internal_gmtime32(dest, time32);
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

errno_t __cdecl _gmtime64_s(struct tm *dest, const __time64_t *source)
{
	if (dest)
	{
		uint64_t time64;

		if (source && (int64_t)(time64 = *source) >= MIN_LOCAL_TIME)
			if ((int64_t)time64 <= INT32_MAX)
				return internal_gmtime32(dest, (uint32_t)time64);
			else if (time64 <= MAX_TIME_T + MAX_LOCAL_TIME)
				return internal_gmtime64(dest, time64);
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

static errno_t internal_gmtime32(struct tm *dest, uint32_t time32)
{
	#define DAY_SEC     (24 * 60 * 60)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define LEAP_DAY    (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	uint32_t year, days;

	time32        = time32 + DAY_SEC;
	dest->tm_sec  = time32 % 60;
	time32        = time32 / 60;
	dest->tm_min  = time32 % 60;
	time32        = time32 / 60;
	dest->tm_hour = time32 % 24;
	time32        = time32 / 24 + SINCE(1970) - (SINCE(1600) + LEAP_DAY) - 1;
	#pragma region { __assume(time32 < YEAR400 * 2); year = time32 / YEAR400 * 400; days = time32 % YEAR400; }
	if (!_subborrow_u32(0, time32, YEAR400, &days)) {
		year = 400;
	} else {
		days += YEAR400;
		year = 0;
	}
	#pragma endregion
	return internal_gmtime_less_than_400_years_left(dest, year, days);

	#undef DAY_SEC
	#undef SINCE
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

static errno_t internal_gmtime64(struct tm *dest, uint64_t time64)
{
	#define DAY_SEC     (24 * 60 * 60)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define LEAP_DAY    (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	uint32_t remainder, year, days;

	remainder     =            time64 % DAY_SEC;
	days          = (uint32_t)(time64 / DAY_SEC) + SINCE(1970) - (SINCE(1600) + LEAP_DAY);
	dest->tm_sec  = remainder % 60;
	remainder     = remainder / 60;
	dest->tm_min  = remainder % 60;
	dest->tm_hour = remainder / 60;
	#pragma region { __assume(days < YEAR400 * 4); year = days / YEAR400 * 400; days %= YEAR400; }
	do {	// do { ... } while (0);
		year = 0;
		if (days < YEAR400 * 2) {
			if (!_subborrow_u32(0, days, YEAR400, &days)) {
				year = 400;
				break;
			}
		} else
			if (!_subborrow_u32(0, days, YEAR400 * 3, &days)) {
				year = 400 * 3;
				break;
			} else
				year = 400 * 2;
		days += YEAR400;
	} while (0);
	#pragma endregion
	return internal_gmtime_less_than_400_years_left(dest, year, days);

	#undef DAY_SEC
	#undef SINCE
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

static errno_t internal_gmtime_less_than_400_years_left(struct tm *dest, uint32_t year, uint32_t days)
{
	#define DIV(dividend, divisor) (((dividend) * ((0x100000000 + (divisor) - 1) / (divisor))) >> 32)
	#define MOD(dividend, divisor) ((dividend) - DIV(dividend, divisor) * (divisor))
	#define DAY_SEC                (24 * 60 * 60)
	#define LEAP_DAY               (31 + 28)
	#define YEAR                   365
	#define YEAR4                  (YEAR * 4 + 1)
	#define YEAR100                (YEAR4 * 25 - 1)
	#define YEAR400                (YEAR100 * 4 + 1)

	static const uint32_t mon_yday[] = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };

	uint32_t leap, mon, yday;

	dest->tm_wday = MOD(days + 2, 7);
	do {	// do { ... } while (0);
		if (!(leap = days < YEAR - LEAP_DAY + 1)) {
			#pragma region { __assume(days < YEAR400); year += days / YEAR100 * 100; days %= YEAR100; }
			do {	// do { ... } while (0);
				if (days < YEAR100 * 2) {
					if (!_subborrow_u32(0, days, YEAR100, &days)) {
						year += 100;
						break;
					}
				} else
					if (!_subborrow_u32(0, days, YEAR100 * 3, &days)) {
						year += 300;
						break;
					} else
						year += 200;
				days += YEAR100;
			} while (0);
			#pragma endregion
			if (days >= YEAR - LEAP_DAY + 1) {
				__assume(days < YEAR100);
				year += days / YEAR4 * 4;
				days %= YEAR4;
				if (!(leap = days < YEAR - LEAP_DAY + 1)) {
					#pragma region { __assume(days < YEAR4); days += LEAP_DAY - 1; year += days / YEAR; days %= YEAR; }
					if (days < YEAR * 2 - LEAP_DAY + 1) {
						if (!_subborrow_u32(0, days, YEAR - LEAP_DAY + 1, &days)) {
							year++;
							break;
						}
					} else
						if (_subborrow_u32(0, days, YEAR * 3 - LEAP_DAY + 1, &days))
							year += 2;
						else
							if (!_subborrow_u32(0, days, YEAR, &days)) {
								year += 4;
								break;
							} else
								year += 3;
					days += YEAR;
					#pragma endregion
					break;
				}
			}
		}
		days += LEAP_DAY - 1 + leap;
	} while (0);
	dest->tm_year  = year - (1900 - 1600);
	dest->tm_yday  = days;
	dest->tm_isdst = 0;
	do {	// do { ... } while (0);
		if ((mon = days / 32) != 1) {
			days -= leap;
			yday = mon_yday[mon + 1];
			if (days <= yday) {
				days -= mon_yday[mon];
				break;
			}
		} else {
			days -= 30;
			yday = leap + 28;
			if (days <= yday)
				break;
		}
		days -= yday;
		mon++;
	} while (0);
	dest->tm_mday = days;
	dest->tm_mon  = mon;
	return 0;

	#undef DIV
	#undef MOD
	#undef DAY_SEC
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}
