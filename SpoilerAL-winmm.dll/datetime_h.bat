@echo off
setlocal
if "%~1" == "" (
	set FileName=datetime.h
) else (
	set FileName=%1
)
set current_date=%date%
set current_time=%time%
set date_yyyy=%current_date:~0,4%
set date_mm=%current_date:~5,2%
set date_dd=%current_date:~8,2%
set time_hh=%current_time:~0,2%
set time_hh=%time_hh: =0%
set time_mm=%current_time:~3,2%
set time_ss=%current_time:~6,2%
set time_fff=%current_time:~9,2%0
set /a date_year=1%date_yyyy%-10000
set /a date_month=1%date_mm%-100
set /a date_day=1%date_dd%-100
set /a time_hour=1%time_hh%-100
set /a time_minute=1%time_mm%-100
set /a time_second=1%time_ss%-100
set /a time_millisecond=1%time_fff%-1000
> %FileName% echo #ifndef _DATETIME_H_
>>%FileName% echo #define _DATETIME_H_
>>%FileName% echo.
>>%FileName% echo #if _MSC_VER ^> 1000
>>%FileName% echo #pragma once
>>%FileName% echo #endif
>>%FileName% echo.
>>%FileName% echo #define __DATE_YEAR__         %date_year%
>>%FileName% echo #define __DATE_MONTH__        %date_month%
>>%FileName% echo #define __DATE_DAY__          %date_day%
>>%FileName% echo #define __TIME_HOUR__         %time_hour%
>>%FileName% echo #define __TIME_MINUTE__       %time_minute%
>>%FileName% echo #define __TIME_SECOND__       %time_second%
>>%FileName% echo #define __TIME_MILLISECOND__  %time_millisecond%
>>%FileName% echo.
>>%FileName% echo #define __DATE_YYYY__         %date_yyyy%
>>%FileName% echo #define __DATE_MM__           %date_mm%
>>%FileName% echo #define __DATE_DD__           %date_dd%
>>%FileName% echo #define __TIME_HH__           %time_hh%
>>%FileName% echo #define __TIME_MM__           %time_mm%
>>%FileName% echo #define __TIME_SS__           %time_ss%
>>%FileName% echo #define __TIME_FFF__          %time_fff%
>>%FileName% echo.
>>%FileName% echo #define __DATETIME_TIME_T__ (                                                       \
>>%FileName% echo     (                                                                               \
>>%FileName% echo           (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) * 365 - 365                        \
>>%FileName% echo         + (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) / 4                                \
>>%FileName% echo         - (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) / 100                              \
>>%FileName% echo         + (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) / 400                              \
>>%FileName% echo         + ((__DATE_MONTH__ + (__DATE_MONTH__ ^< 3) * 12) * 979 - 1033) / 32          \
>>%FileName% echo         + __DATE_DAY__ - 1                                                          \
>>%FileName% echo         - ((1970 - 1) * 365 + (1970 - 1) / 4 - (1970 - 1) / 100 + (1970 - 1) / 400) \
>>%FileName% echo     ) * 86400                                                                       \
>>%FileName% echo     + __TIME_HOUR__ * 3600                                                          \
>>%FileName% echo     + __TIME_MINUTE__ * 60                                                          \
>>%FileName% echo     + __TIME_SECOND__)
>>%FileName% echo.
>>%FileName% echo #define __DATETIME_FILETIME__ (                                                     \
>>%FileName% echo     (                                                                               \
>>%FileName% echo           (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) * 365 - 365                        \
>>%FileName% echo         + (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) / 4                                \
>>%FileName% echo         - (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) / 100                              \
>>%FileName% echo         + (__DATE_YEAR__ - (__DATE_MONTH__ ^< 3)) / 400                              \
>>%FileName% echo         + ((__DATE_MONTH__ + (__DATE_MONTH__ ^< 3) * 12) * 979 - 1033) / 32          \
>>%FileName% echo         + __DATE_DAY__ - 1                                                          \
>>%FileName% echo         - ((1601 - 1) * 365 + (1601 - 1) / 4 - (1601 - 1) / 100 + (1601 - 1) / 400) \
>>%FileName% echo     ) * 864000000000                                                                \
>>%FileName% echo     + __TIME_HOUR__ * 36000000000                                                   \
>>%FileName% echo     + __TIME_MINUTE__ * 600000000                                                   \
>>%FileName% echo     + __TIME_SECOND__ * 10000000                                                    \
>>%FileName% echo     + __TIME_MILLISECOND__ * 10000)
>>%FileName% echo.
>>%FileName% echo #endif	// _DATETIME_H_
endlocal
