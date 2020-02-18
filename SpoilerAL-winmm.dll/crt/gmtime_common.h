{
	#ifndef SIZE_OF_TIME
		#error "SIZE_OF_TIME must be defined."
	#endif

	#define DIV(dividend, divisor) (((dividend) * ((0x100000000 + (divisor) - 1) / (divisor))) >> 32)
	#define MOD(dividend, divisor) ((dividend) - DIV(dividend, divisor) * (divisor))
	#define DAY_SEC                (60 * 60 * 24)
	#define SINCE(year)            (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define JAN_FEB                (31 + 28)
	#define YEAR                   365
	#define YEAR4                  (YEAR * 4 + 1)
	#define YEAR100                (YEAR4 * 25 - 1)
	#define YEAR400                (YEAR100 * 4 + 1)

#if SIZE_OF_TIME > 4
	uint32_t remainder;
#endif
	uint32_t year, days, leap;

#if SIZE_OF_TIME > 4
	remainder     = time64 % DAY_SEC;
	time64        = time64 / DAY_SEC + (SINCE(1970) - SINCE(1600) - JAN_FEB);
	dest->tm_sec  = remainder % 60;
	remainder     = remainder / 60;
	dest->tm_min  = remainder % 60;
	dest->tm_hour = remainder / 60;
	year          = (uint32_t)(time64 / YEAR400) * 400;
	days          =            time64 % YEAR400;
#else
	time32        = time32 + DAY_SEC;
	dest->tm_sec  = time32 % 60;
	time32        = time32 / 60;
	dest->tm_min  = time32 % 60;
	time32        = time32 / 60;
	dest->tm_hour = time32 % 24;
	time32        = time32 / 24 + (SINCE(1970) - SINCE(1600) - JAN_FEB - 1);
	if (!_subborrow_u32(0, time32, YEAR400, &days)) {
		year = 400;
	} else {
		days += YEAR400;
		year = 0;
	}
#endif
	dest->tm_wday = MOD(days + 2, 7);
	do {	// do { ... } while (0);
		if (!(leap = days < YEAR - JAN_FEB + 1)) {
			year += 300;
			if (_subborrow_u32(0, days, YEAR100 * 3, &days)) {
				year -= 100;
				if (!_addcarry_u32(0, days, YEAR100, &days)) {
					year -= 100;
					if (!_addcarry_u32(0, days, YEAR100, &days)) {
						year -= 100;
						days += YEAR100;
					}
				}
			}
			if (days >= YEAR - JAN_FEB + 1) {
				year += days / YEAR4 * 4;
				days =  days % YEAR4;
				if (!(leap = days < YEAR - JAN_FEB + 1)) {
					days += JAN_FEB - 1;
					year += 4;
					if (_subborrow_u32(0, days, YEAR * 4, &days)) {
						year--;
						if (!_addcarry_u32(0, days, YEAR, &days)) {
							year--;
							if (!_addcarry_u32(0, days, YEAR, &days)) {
								year--;
								if (!_addcarry_u32(0, days, YEAR, &days)) {
									year--;
									days += YEAR;
								}
							}
						}
					}
					break;
				}
			}
		}
		days += JAN_FEB - 1 + leap;
	} while (0);
	dest->tm_year  = year - (1900 - 1600);
	dest->tm_yday  = days;
	dest->tm_isdst = 0;
	switch (dest->tm_mon = days / 32) {
	case  0: if (_subborrow_u32(0, days       ,   31, &days)) { dest->tm_mday = days +   32; return 0; } break;
	case  1: days -= 30; leap += 29;
	         if (_subborrow_u32(0, days       , leap, &days)) { dest->tm_mday = days + leap; return 0; } break;
	case  2: if (_subborrow_u32(0, days - leap,   90, &days)) { dest->tm_mday = days +   32; return 0; } break;
	case  3: if (_subborrow_u32(0, days - leap,  120, &days)) { dest->tm_mday = days +   31; return 0; } break;
	case  4: if (_subborrow_u32(0, days - leap,  151, &days)) { dest->tm_mday = days +   32; return 0; } break;
	case  5: if (_subborrow_u32(0, days - leap,  181, &days)) { dest->tm_mday = days +   31; return 0; } break;
	case  6: if (_subborrow_u32(0, days - leap,  212, &days)) { dest->tm_mday = days +   32; return 0; } break;
	case  7: if (_subborrow_u32(0, days - leap,  243, &days)) { dest->tm_mday = days +   32; return 0; } break;
	case  8: if (_subborrow_u32(0, days - leap,  273, &days)) { dest->tm_mday = days +   31; return 0; } break;
	case  9: if (_subborrow_u32(0, days - leap,  304, &days)) { dest->tm_mday = days +   32; return 0; } break;
	case 10: if (_subborrow_u32(0, days - leap,  334, &days)) { dest->tm_mday = days +   31; return 0; } break;
	case 11:       dest->tm_mday = days - leap - 333;                                        return 0;
	default: __assume(0);
	}
	dest->tm_mon++;
	dest->tm_mday = days + 1;
	return 0;

	#undef DIV
	#undef MOD
	#undef DAY_SEC
	#undef SINCE
	#undef JAN_FEB
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}
