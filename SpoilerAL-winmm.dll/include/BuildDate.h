#ifndef _BUILDDATE_H_
#define _BUILDDATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define BUILD_YEAR_CH0 __DATE__[ 7]
#define BUILD_YEAR_CH1 __DATE__[ 8]
#define BUILD_YEAR_CH2 __DATE__[ 9]
#define BUILD_YEAR_CH3 __DATE__[10]

#define BUILD_YEAR_DIG0 (BUILD_YEAR_CH0 - '0')
#define BUILD_YEAR_DIG1 (BUILD_YEAR_CH1 - '0')
#define BUILD_YEAR_DIG2 (BUILD_YEAR_CH2 - '0')
#define BUILD_YEAR_DIG3 (BUILD_YEAR_CH3 - '0')

#define BUILD_YEAR (         \
    BUILD_YEAR_DIG0 * 1000 + \
    BUILD_YEAR_DIG1 *  100 + \
    BUILD_YEAR_DIG2 *   10 + \
    BUILD_YEAR_DIG3)

#define BUILD_MONTH_IS_JAN (*(unsigned long *)__DATE__ == ' naJ')
#define BUILD_MONTH_IS_FEB (*(unsigned long *)__DATE__ == ' beF')
#define BUILD_MONTH_IS_MAR (*(unsigned long *)__DATE__ == ' raM')
#define BUILD_MONTH_IS_APR (*(unsigned long *)__DATE__ == ' rpA')
#define BUILD_MONTH_IS_MAY (*(unsigned long *)__DATE__ == ' yaM')
#define BUILD_MONTH_IS_JUN (*(unsigned long *)__DATE__ == ' nuJ')
#define BUILD_MONTH_IS_JUL (*(unsigned long *)__DATE__ == ' luJ')
#define BUILD_MONTH_IS_AUG (*(unsigned long *)__DATE__ == ' guA')
#define BUILD_MONTH_IS_SEP (*(unsigned long *)__DATE__ == ' peS')
#define BUILD_MONTH_IS_OCT (*(unsigned long *)__DATE__ == ' tcO')
#define BUILD_MONTH_IS_NOV (*(unsigned long *)__DATE__ == ' voN')
#define BUILD_MONTH_IS_DEC (*(unsigned long *)__DATE__ == ' ceD')

#define BUILD_MONTH_DIG0 (                  \
    *(unsigned long *)__DATE__ == ' tcO' || \
    *(unsigned long *)__DATE__ == ' voN' || \
    *(unsigned long *)__DATE__ == ' ceD')

#define BUILD_MONTH_DIG1 (                                      \
    *(unsigned long *)__DATE__ <= ' peS' ?                      \
        *(unsigned long *)__DATE__ != ' peS' ?                  \
            *(unsigned long *)__DATE__ <= ' luJ' ?              \
                *(unsigned long *)__DATE__ != ' luJ' ?          \
                    *(unsigned long *)__DATE__ != ' beF' &&     \
                    *(unsigned long *)__DATE__ != ' ceD' ?      \
                        8 :                                     \
                        2 :                                     \
                    7 :                                         \
                *(unsigned long *)__DATE__ != ' naJ' ?          \
                    6 :                                         \
                    1 :                                         \
            9 :                                                 \
        *(unsigned long *)__DATE__ <= ' tcO' ?                  \
            *(unsigned long *)__DATE__ != ' tcO' ?              \
                (*(unsigned long *)__DATE__ != ' raM') + 3 :    \
                0 :                                             \
            *(unsigned long *)__DATE__ != ' voN' ?              \
                5 :                                             \
                1)

#define BUILD_MONTH (                                                       \
    *(unsigned long *)__DATE__ <= ' peS' ?                                  \
        *(unsigned long *)__DATE__ != ' peS' ?                              \
            *(unsigned long *)__DATE__ <= ' luJ' ?                          \
                *(unsigned long *)__DATE__ != ' luJ' ?                      \
                    *(unsigned long *)__DATE__ != ' beF' ?                  \
                        (*(unsigned long *)__DATE__ == ' ceD') * 4 + 8 :    \
                        2 :                                                 \
                    7 :                                                     \
                *(unsigned long *)__DATE__ != ' naJ' ?                      \
                    6 :                                                     \
                    1 :                                                     \
            9 :                                                             \
        *(unsigned long *)__DATE__ <= ' tcO' ?                              \
            *(unsigned long *)__DATE__ != ' tcO' ?                          \
                (*(unsigned long *)__DATE__ != ' raM') + 3 :                \
                10 :                                                        \
            *(unsigned long *)__DATE__ != ' voN' ?                          \
                5 :                                                         \
                11)

#define BUILD_MONTH_CH0 (BUILD_MONTH_DIG0 + '0')
#define BUILD_MONTH_CH1 (BUILD_MONTH_DIG1 + '0')

#define BUILD_DAY_CH0 (__DATE__[4] >= '0' ? __DATE__[4] : '0')
#define BUILD_DAY_CH1 __DATE__[5]

#define BUILD_DAY_DIG0 (__DATE__[4] >= '0' ? __DATE__[4] - '0' : 0)
#define BUILD_DAY_DIG1 (__DATE__[5] - '0')

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
	*(unsigned __int64 *)(filetime) = YEARMONTHDAY_TO_FILETIME_UI64(year, month, day)

#define BUILD_DATE_FILETIME_UI64 \
	YEARMONTHDAY_TO_FILETIME_UI64(BUILD_YEAR, BUILD_MONTH, BUILD_DAY)

#define GET_FILETIME_FROM_BUILD_DATE(filetime) \
	*(unsigned __int64 *)(filetime) = BUILD_DATE_FILETIME_UI64

#endif	// _BUILDDATE_H_
