@echo off
setlocal
if "%~1" == "" (
	set FileName=datetime.h
) else (
	set FileName=%1
)
::
:: PowerShell requires clear the "env" of environment variable.
::
set env=
::
:: #define SINCE(year) ( ((year) - 1) * 365 \
::                     + ((year) - 1) / 4   \
::                     - ((year) - 1) / 100 \
::                     + ((year) - 1) / 400)
:: #define DAY_SEC     86400           // Seconds per day
:: #define DAY_HNS     864000000000    // Hundred nano seconds per day
::
:: assert(0x0701CE1722770000 == SINCE(1601) * DAY_HNS);
:: assert(11644473600 == (SINCE(1970) - SINCE(1601)) * UINT64_C(DAY_SEC));
::
set command=
set command=%command% $now = Get-Date -Year 1970 -Month 1 -Day 1 -Hour 9 -Minute 0 -Second 0 -Millisecond 0;
set command=%command% $ft = $now.ToUniversalTime().Ticks - 0x0701CE1722770000;
set command=%command% $tm = (($ft - $ft %% 10000000) / 10000000) - 11644473600;
set command=%command% return $now.ToString('yyyyy MM dd MMMM dddd HH mm ss fffffff', [CultureInfo]'en-US') +
set command=%command%     ' ' + $now.DayOfWeek.ToString('d') +
set command=%command%     ' ' + $now.DayOfYear.ToString() +
set command=%command%     ' 0x' + $tm.ToString('X16') +
set command=%command%     ' 0x' + $ft.ToString('X16');
for /f "usebackq tokens=1-14 delims= " %%a in (`powershell.exe -Command "%command%"`) do (
	set date_yyyyy=%%a
	set date_mm=%%b
	set date_dd=%%c
	set date_mmmm=%%d
	set date_dddd=%%e
	set time_hh=%%f
	set time_mm=%%g
	set time_ss=%%h
	set time_fffffff=%%i
	set date_dayofweek=%%j
	set date_dayofyear=%%k
	set tm=%%l
	set ft=%%m
)
set /a date_year = 1%date_yyyyy% - 100000
set /a date_month = 1%date_mm% - 100
set /a date_day = 1%date_dd% - 100
set /a time_hour = 1%time_hh% - 100
set /a time_minute = 1%time_mm% - 100
set /a time_second = 1%time_ss% - 100
set /a time_millisecond = 1%time_fffffff:~,3% - 1000
set /a time_microsecond = 1%time_fffffff:~3,3% - 1000
set /a time_nanosecond = %time_fffffff:~-1% * 100
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
>>%FileName% echo #define __DATE_DAYOFWEEK__    %date_dayofweek%
>>%FileName% echo #define __DATE_DAYOFYEAR__    %date_dayofyear%
>>%FileName% echo #define __TIME_HOUR__         %time_hour%
>>%FileName% echo #define __TIME_MINUTE__       %time_minute%
>>%FileName% echo #define __TIME_SECOND__       %time_second%
>>%FileName% echo #define __TIME_MILLISECOND__  %time_millisecond%
>>%FileName% echo #define __TIME_MICROSECOND__  %time_microsecond%
>>%FileName% echo #define __TIME_NANOSECOND__   %time_nanosecond%
>>%FileName% echo.
>>%FileName% echo #define __DATE_YYYY__         %date_yyyyy:~-4%
>>%FileName% echo #define __DATE_MM__           %date_mm%
>>%FileName% echo #define __DATE_DD__           %date_dd%
>>%FileName% echo #define __DATE_MMM__          %date_mmmm:~,3%
>>%FileName% echo #define __DATE_MMMM__         %date_mmmm%
>>%FileName% echo #define __DATE_DDD__          %date_dddd:~,3%
>>%FileName% echo #define __DATE_DDDD__         %date_dddd%
>>%FileName% echo #define __TIME_HH__           %time_hh%
>>%FileName% echo #define __TIME_MM__           %time_mm%
>>%FileName% echo #define __TIME_SS__           %time_ss%
>>%FileName% echo #define __TIME_F__            %time_fffffff:~,1%
>>%FileName% echo #define __TIME_FF__           %time_fffffff:~,2%
>>%FileName% echo #define __TIME_FFF__          %time_fffffff:~,3%
>>%FileName% echo #define __TIME_FFFF__         %time_fffffff:~,4%
>>%FileName% echo #define __TIME_FFFFF__        %time_fffffff:~,5%
>>%FileName% echo #define __TIME_FFFFFF__       %time_fffffff:~,6%
>>%FileName% echo #define __TIME_FFFFFFF__      %time_fffffff%
>>%FileName% echo.
>>%FileName% echo #define __DATETIME_TIME_T__   %tm%
>>%FileName% echo #define __DATETIME_FILETIME__ %ft%
>>%FileName% echo.
>>%FileName% echo #endif	// _DATETIME_H_
endlocal
