#ifndef _BUILDDATE_H_
#define _BUILDDATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define BUILD_YEAR_CH0 __DATE__[ 7]
#define BUILD_YEAR_CH1 __DATE__[ 8]
#define BUILD_YEAR_CH2 __DATE__[ 9]
#define BUILD_YEAR_CH3 __DATE__[10]

#define BUILD_YEAR_DIG0 (size_t)(BUILD_YEAR_CH0 - '0')
#define BUILD_YEAR_DIG1 (size_t)(BUILD_YEAR_CH1 - '0')
#define BUILD_YEAR_DIG2 (size_t)(BUILD_YEAR_CH2 - '0')
#define BUILD_YEAR_DIG3 (size_t)(BUILD_YEAR_CH3 - '0')

#define BUILD_YEAR (         \
    BUILD_YEAR_DIG0 * 1000 + \
    BUILD_YEAR_DIG1 *  100 + \
    BUILD_YEAR_DIG2 *   10 + \
    BUILD_YEAR_DIG3)

#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[2] == 'G')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')

#define BUILD_MONTH_DIG0 \
    (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D')

#define BUILD_MONTH_DIG1 (     \
    (__DATE__[0] == 'F' ||     \
     __DATE__[0] == 'D') ? 2 : \
     __DATE__[0] == 'S'  ? 9 : \
     __DATE__[0] == 'O'  ? 0 : \
     __DATE__[2] == 'y'  ? 5 : \
     __DATE__[2] == 'l'  ? 7 : \
     __DATE__[2] == 'g'  ? 8 : \
     __DATE__[0] == 'M'  ? 3 : \
     __DATE__[0] == 'A'  ? 4 : \
     __DATE__[1] == 'u'  ? 6 : \
                           1)

#define BUILD_MONTH (                  \
    *(long *)__DATE__ == ' naJ' ?  1 : \
    *(long *)__DATE__ == ' beF' ?  2 : \
    *(long *)__DATE__ == ' raM' ?  3 : \
    *(long *)__DATE__ == ' rpA' ?  4 : \
    *(long *)__DATE__ == ' yaM' ?  5 : \
    *(long *)__DATE__ == ' nuJ' ?  6 : \
    *(long *)__DATE__ == ' luJ' ?  7 : \
    *(long *)__DATE__ == ' guA' ?  8 : \
    *(long *)__DATE__ == ' peS' ?  9 : \
    *(long *)__DATE__ == ' tcO' ? 10 : \
    *(long *)__DATE__ == ' voN' ? 11 : \
                                  12)

#define BUILD_MONTH_CH0 (BUILD_MONTH_DIG0 + '0')
#define BUILD_MONTH_CH1 (BUILD_MONTH_DIG1 + '0')

#define BUILD_DAY_CH0 (__DATE__[4] >= '0' ? __DATE__[4] : '0')
#define BUILD_DAY_CH1 __DATE__[5]

#define BUILD_DAY_DIG0 (size_t)(BUILD_DAY_CH0 - '0')
#define BUILD_DAY_DIG1 (size_t)(BUILD_DAY_CH1 - '0')

#define BUILD_DAY (       \
    BUILD_DAY_DIG0 * 10 + \
    BUILD_DAY_DIG1)

#define BUILD_YEAR_ARRAY \
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3

#define BUILD_MONTH_ARRAY \
    BUILD_MONTH_CH0, BUILD_MONTH_CH1

#define BUILD_DAY_ARRAY \
    BUILD_DAY_CH0, BUILD_DAY_CH1

#define BUILD_DATE_ARRAY(sep1, sep2) \
    BUILD_YEAR_ARRAY,                \
    sep1,                            \
    BUILD_MONTH_ARRAY,               \
    sep2,                            \
    BUILD_DAY_ARRAY

#define YEARMONTHDAY_TO_FILETIME_UI64(year, month, day) (                           \
    (                                                                               \
          ((year) - (month) < 3) * 365 - 365                                        \
        + ((year) - (month) < 3) / 4                                                \
        - ((year) - (month) < 3) / 100                                              \
        + ((year) - (month) < 3) / 400                                              \
        + (((month) + ((month) < 3) * 12) * 979 - 1033) / 32                        \
        + (day) - 1                                                                 \
        - ((1601 - 1) * 365 + (1601 - 1) / 4 - (1601 - 1) / 100 + (1601 - 1) / 400) \
    )                                                                               \
    * ((unsigned __int64)(60 * 60 * 24) * (1000 * 1000 * 1000 / 100)))

#define GET_FILETIME_FROM_YEARMONTHDAY(filetime, year, month, day) \
	*(unsigned __int64 *)&(filetime) = YEARMONTHDAY_TO_FILETIME_UI64(year, month, day)

#define BUILD_DATE_FILETIME_UI64 \
	YEARMONTHDAY_TO_FILETIME_UI64(BUILD_YEAR, BUILD_MONTH, BUILD_DAY)

#define GET_FILETIME_FROM_BUILD_DATE(filetime) \
	*(unsigned __int64 *)&(filetime) = BUILD_DATE_FILETIME_UI64

#endif	// _BUILDDATE_H_
