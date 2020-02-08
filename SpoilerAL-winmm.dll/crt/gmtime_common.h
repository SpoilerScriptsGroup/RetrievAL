{
	#define YEAR      365
	#define YEAR4     (YEAR * 4 + 1)
	#define YEAR100   (YEAR4 * 25 - 1)
	#define YEAR400   (YEAR100 * 4 + 1)
	#define SINCE1970 (YEAR400 * 4 + YEAR100 * 3 + YEAR4 * 17 + YEAR * 2)
	#define LEAP_DAY  (31 + 29)

	uint32_t year, day, quotient;

	dest->tm_sec  =  (uint32_t)(time % 60)           ; time =             time / 60                ;
	dest->tm_min  =  (uint32_t)(time % 60)           ; time =             time / 60                ;
	dest->tm_hour =  (uint32_t)(time % 24)           ; time =             time / 24  + 4           ;
	dest->tm_wday =  (uint32_t)(time %  7)           ; time += (SINCE1970 % YEAR400) - 4 - LEAP_DAY;
	year          =  (uint32_t)(time / YEAR400) * 400; day  =  (uint32_t)(time % YEAR400)          ;
	year          +=            day  / YEAR100  * 100; day  =             day  % YEAR100           ;
	year          +=            day  / YEAR4    *   4; day  =             day  % YEAR4   + LEAP_DAY;
	year          +=            day  / YEAR          ; day  =             day  % YEAR              ;
	dest->tm_year =  year - (1970 % 400) + 1970 - 1900;
	dest->tm_yday =  day;
	if      (_subborrow_u32(0, day, 31, &day)) { dest->tm_mon =  0; dest->tm_mday = day + 32; }
	else if (_subborrow_u32(0, day, 28, &day)) { dest->tm_mon =  1; dest->tm_mday = day + 29; }
	else if (!(year % 4) && (                           // year %   4 == 0
	         (quotient = year / 100) * 100 != year ||   // year % 100 != 0
	         (quotient & -4)         * 100 == year) &&  // year % 400 == 0
	         _subborrow_u32(0, day,  1, &day)) { dest->tm_mon =  1; dest->tm_mday = 29      ; }
	else if (_subborrow_u32(0, day, 31, &day)) { dest->tm_mon =  2; dest->tm_mday = day + 32; }
	else if (_subborrow_u32(0, day, 30, &day)) { dest->tm_mon =  3; dest->tm_mday = day + 31; }
	else if (_subborrow_u32(0, day, 31, &day)) { dest->tm_mon =  4; dest->tm_mday = day + 32; }
	else if (_subborrow_u32(0, day, 30, &day)) { dest->tm_mon =  5; dest->tm_mday = day + 31; }
	else if (_subborrow_u32(0, day, 31, &day)) { dest->tm_mon =  6; dest->tm_mday = day + 32; }
	else if (_subborrow_u32(0, day, 31, &day)) { dest->tm_mon =  7; dest->tm_mday = day + 32; }
	else if (_subborrow_u32(0, day, 30, &day)) { dest->tm_mon =  8; dest->tm_mday = day + 31; }
	else if (_subborrow_u32(0, day, 31, &day)) { dest->tm_mon =  9; dest->tm_mday = day + 32; }
	else if (_subborrow_u32(0, day, 30, &day)) { dest->tm_mon = 10; dest->tm_mday = day + 31; }
	else                                       { dest->tm_mon = 11; dest->tm_mday = day +  1; }
	dest->tm_isdst = 0;
	return 0;

	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
	#undef SINCE1970
	#undef LEAP_DAY
}
