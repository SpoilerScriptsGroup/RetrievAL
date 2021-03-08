@echo off
setlocal
if "%~1" == "" (
	set FileName=datetime.h
) else (
	set FileName=%1
)
::
:: PowerShell requires clear the environment variable of "env".
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
set command=%command% $d = Get-Date;
set command=%command% $ft = $d.ToUniversalTime().Ticks - 0x0701CE1722770000;
set command=%command% $tm = (($ft - $ft %% 10000000) / 10000000) - 11644473600;
set command=%command% return $d.ToString('yyyyy MM dd HH mm ss fffffff') +
set command=%command%     ' 0x' + $tm.ToString('X16') +
set command=%command%     ' 0x' + $ft.ToString('X16');
for /f "usebackq tokens=1-9 delims= " %%a in (`powershell.exe -Command "%command%"`) do (
	set date_yyyyy=%%a
	set date_mm=%%b
	set date_dd=%%c
	set time_hh=%%d
	set time_mm=%%e
	set time_ss=%%f
	set time_fffffff=%%g
	set tm=%%h
	set ft=%%i
)
set date_yyyy=%date_yyyyy:~-4%
set /a date_year=1%date_yyyyy%-100000
set /a date_month=1%date_mm%-100
set /a date_day=1%date_dd%-100
set /a time_hour=1%time_hh%-100
set /a time_minute=1%time_mm%-100
set /a time_second=1%time_ss%-100
set /a time_millisecond=1%time_fffffff:~,3%-1000
set /a time_microsecond=1%time_fffffff:~3,3%-1000
set /a time_nanosecond=%time_fffffff:~-1%*100
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
>>%FileName% echo #define __TIME_MICROSECOND__  %time_microsecond%
>>%FileName% echo #define __TIME_NANOSECOND__   %time_nanosecond%
>>%FileName% echo.
>>%FileName% echo #define __DATE_YYYY__         %date_yyyy%
>>%FileName% echo #define __DATE_MM__           %date_mm%
>>%FileName% echo #define __DATE_DD__           %date_dd%
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
