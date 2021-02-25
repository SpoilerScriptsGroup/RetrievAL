#ifndef _DATETIME_H_
#define _DATETIME_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define __DATE_YEAR__         1970
#define __DATE_MONTH__        1
#define __DATE_DAY__          1
#define __TIME_HOUR__         0
#define __TIME_MINUTE__       0
#define __TIME_SECOND__       0
#define __TIME_MILLISECOND__  0

#define __DATE_YYYY__         1970
#define __DATE_MM__           01
#define __DATE_DD__           01
#define __TIME_HH__           00
#define __TIME_MM__           00
#define __TIME_SS__           00
#define __TIME_FFF__          000

#define __DATETIME_TIME_T__ (                                                       \
    (                                                                               \
          (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) * 365 - 365                        \
        + (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) / 4                                \
        - (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) / 100                              \
        + (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) / 400                              \
        + ((__DATE_MONTH__ + (__DATE_MONTH__ < 3) * 12) * 979 - 1033) / 32          \
        + __DATE_DAY__ - 1                                                          \
        - ((1970 - 1) * 365 + (1970 - 1) / 4 - (1970 - 1) / 100 + (1970 - 1) / 400) \
    ) * 86400                                                                       \
    + __TIME_HOUR__ * 3600                                                          \
    + __TIME_MINUTE__ * 60                                                          \
    + __TIME_SECOND__)

#define __DATETIME_FILETIME__ (                                                     \
    (                                                                               \
          (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) * 365 - 365                        \
        + (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) / 4                                \
        - (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) / 100                              \
        + (__DATE_YEAR__ - (__DATE_MONTH__ < 3)) / 400                              \
        + ((__DATE_MONTH__ + (__DATE_MONTH__ < 3) * 12) * 979 - 1033) / 32          \
        + __DATE_DAY__ - 1                                                          \
        - ((1601 - 1) * 365 + (1601 - 1) / 4 - (1601 - 1) / 100 + (1601 - 1) / 400) \
    ) * 864000000000                                                                \
    + __TIME_HOUR__ * 36000000000                                                   \
    + __TIME_MINUTE__ * 600000000                                                   \
    + __TIME_SECOND__ * 10000000                                                    \
    + __TIME_MILLISECOND__ * 10000)

#endif	// _DATETIME_H_
