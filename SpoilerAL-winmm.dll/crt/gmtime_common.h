{
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define JAN_FEB     (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	uint32_t year, days;
	int32_t leap;

	dest->tm_sec  = (uint32_t)(time % 60);
	time          =            time / 60 ;
	dest->tm_min  = (uint32_t)(time % 60);
	time          =            time / 60 ;
	dest->tm_hour = (uint32_t)(time % 24);
	time          =            time / 24 + 4;
	dest->tm_wday = (uint32_t)(time %  7);
	time += SINCE(1970) - SINCE(1600) - JAN_FEB - 4;
	year =  (uint32_t)(time / YEAR400) * 400;
	days =  (uint32_t)(time % YEAR400);
	if (!(leap = days < YEAR - JAN_FEB + 1)) {
		year += days / YEAR100 * 100;
		days =  days % YEAR100;
		if (days >= YEAR - JAN_FEB + 1) {
			year += days / YEAR4 * 4;
			days =  days % YEAR4;
			leap =  days < YEAR - JAN_FEB + 1;
		}
	}
	days += JAN_FEB - 1;
	year += days / YEAR;
	days =  days % YEAR + leap;
	dest->tm_year = year - (1900 - 1600);
	dest->tm_yday = days;
	if      (_subborrow_u32(0, days, 31  , &days)) { dest->tm_mon =  0; dest->tm_mday = days + 32; }
	else if (_subborrow_u32(0, days, 28  , &days)) { dest->tm_mon =  1; dest->tm_mday = days + 29; }
	else if (_subborrow_u32(0, days, leap, &days)) { dest->tm_mon =  1; dest->tm_mday =        29; }
	else if (_subborrow_u32(0, days, 31  , &days)) { dest->tm_mon =  2; dest->tm_mday = days + 32; }
	else if (_subborrow_u32(0, days, 30  , &days)) { dest->tm_mon =  3; dest->tm_mday = days + 31; }
	else if (_subborrow_u32(0, days, 31  , &days)) { dest->tm_mon =  4; dest->tm_mday = days + 32; }
	else if (_subborrow_u32(0, days, 30  , &days)) { dest->tm_mon =  5; dest->tm_mday = days + 31; }
	else if (_subborrow_u32(0, days, 31  , &days)) { dest->tm_mon =  6; dest->tm_mday = days + 32; }
	else if (_subborrow_u32(0, days, 31  , &days)) { dest->tm_mon =  7; dest->tm_mday = days + 32; }
	else if (_subborrow_u32(0, days, 30  , &days)) { dest->tm_mon =  8; dest->tm_mday = days + 31; }
	else if (_subborrow_u32(0, days, 31  , &days)) { dest->tm_mon =  9; dest->tm_mday = days + 32; }
	else if (_subborrow_u32(0, days, 30  , &days)) { dest->tm_mon = 10; dest->tm_mday = days + 31; }
	else                                           { dest->tm_mon = 11; dest->tm_mday = days +  1; }
	dest->tm_isdst = 0;
	return 0;

	#undef SINCE
	#undef JAN_FEB
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}
