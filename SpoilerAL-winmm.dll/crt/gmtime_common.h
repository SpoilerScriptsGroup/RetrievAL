{
	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define JAN_FEB     (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	uint32_t remainder, year, days, leap;

	remainder     = time % DAY_SEC;
	time          = time / DAY_SEC + (SINCE(1970) - SINCE(1600) - JAN_FEB);
	dest->tm_sec  = remainder % 60;
	remainder     = remainder / 60;
	dest->tm_min  = remainder % 60;
	dest->tm_hour = remainder / 60;
	year          = (uint32_t)(time / YEAR400) * 400;
	days          =            time % YEAR400;
	dest->tm_wday = (days + 2) % 7;
	do {
		if (!(leap = days < YEAR - JAN_FEB + 1)) {
			year += days / YEAR100 * 100;
			days =  days % YEAR100;
			if (days >= YEAR - JAN_FEB + 1) {
				year += days / YEAR4 * 4;
				days =  days % YEAR4;
				if (!(leap = days < YEAR - JAN_FEB + 1)) {
					days += JAN_FEB - 1;
					year += days / YEAR;
					days =  days % YEAR;
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
	case  0: if (_subborrow_u32(0, days       ,   31, &days)) { dest->tm_mday = days + 32; return 0; } break;
	case  1: if (_subborrow_u32(0, days       ,   59, &days)) { dest->tm_mday = days + 29; return 0; } else
	         if (_subborrow_u32(0, days       , leap, &days)) { dest->tm_mday =        29; return 0; } break;
	case  2: if (_subborrow_u32(0, days - leap,   90, &days)) { dest->tm_mday = days + 32; return 0; } break;
	case  3: if (_subborrow_u32(0, days - leap,  120, &days)) { dest->tm_mday = days + 31; return 0; } break;
	case  4: if (_subborrow_u32(0, days - leap,  151, &days)) { dest->tm_mday = days + 32; return 0; } break;
	case  5: if (_subborrow_u32(0, days - leap,  181, &days)) { dest->tm_mday = days + 31; return 0; } break;
	case  6: if (_subborrow_u32(0, days - leap,  212, &days)) { dest->tm_mday = days + 32; return 0; } break;
	case  7: if (_subborrow_u32(0, days - leap,  243, &days)) { dest->tm_mday = days + 32; return 0; } break;
	case  8: if (_subborrow_u32(0, days - leap,  273, &days)) { dest->tm_mday = days + 31; return 0; } break;
	case  9: if (_subborrow_u32(0, days - leap,  304, &days)) { dest->tm_mday = days + 32; return 0; } break;
	case 10: if (_subborrow_u32(0, days - leap,  334, &days)) { dest->tm_mday = days + 31; return 0; } break;
	case 11:       dest->tm_mday = days - leap - 333;                                      return 0;
	default: __assume(0);
	}
	dest->tm_mon++;
	dest->tm_mday = days + 1;
	return 0;

	#undef DAY_SEC
	#undef SINCE
	#undef JAN_FEB
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}
