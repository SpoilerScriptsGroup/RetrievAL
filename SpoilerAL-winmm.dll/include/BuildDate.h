#ifndef _BUILDDATE_H_
#define _BUILDDATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define BUILD_YEAR_CH0 __DATE__[ 7]
#define BUILD_YEAR_CH1 __DATE__[ 8]
#define BUILD_YEAR_CH2 __DATE__[ 9]
#define BUILD_YEAR_CH3 __DATE__[10]

#define BUILD_YEAR_DIGIT (                  \
    (size_t)(BUILD_YEAR_CH0 - '0') * 1000 + \
    (size_t)(BUILD_YEAR_CH1 - '0') *  100 + \
    (size_t)(BUILD_YEAR_CH2 - '0') *   10 + \
    (size_t)(BUILD_YEAR_CH3 - '0'))

#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')

#define BUILD_MONTH_DIG0 \
    (BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC)

#define BUILD_MONTH_DIG1 (                           \
     BUILD_MONTH_IS_OCT                        ? 0 : \
    (BUILD_MONTH_IS_JAN || BUILD_MONTH_IS_NOV) ? 1 : \
    (BUILD_MONTH_IS_FEB || BUILD_MONTH_IS_DEC) ? 2 : \
     BUILD_MONTH_IS_MAR                        ? 3 : \
     BUILD_MONTH_IS_APR                        ? 4 : \
     BUILD_MONTH_IS_MAY                        ? 5 : \
     BUILD_MONTH_IS_JUN                        ? 6 : \
     BUILD_MONTH_IS_JUL                        ? 7 : \
     BUILD_MONTH_IS_AUG                        ? 8 : \
                                                 9)

#define BUILD_MONTH_DIGIT (           \
    (size_t)(BUILD_MONTH_DIG0) * 10 + \
    (size_t)(BUILD_MONTH_DIG1))

#define BUILD_MONTH_CH0 (BUILD_MONTH_DIG0 + '0')
#define BUILD_MONTH_CH1 (BUILD_MONTH_DIG1 + '0')

#define BUILD_DAY_CH0 (__DATE__[4] >= '0' ? __DATE__[4] : '0')
#define BUILD_DAY_CH1 __DATE__[5]

#define BUILD_DAY_DIGIT (                \
    (size_t)(BUILD_DAY_CH0 - '0') * 10 + \
    (size_t)(BUILD_DAY_CH1 - '0'))

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

#endif	// _BUILDDATE_H_
