@echo off
setlocal
if "%~1" == "" (
	set FileName=datetime.h
) else (
	set FileName=%1
)
set env=
set command=$d = Get-Date;
set command=%command% $d = Get-Date;
set command=%command% $ft = $d.Ticks - 0x0701CE1722770000;
set command=%command% $tm = [uint64]($ft / 10000000) - 11644473600;
set command=%command% Write-Host(\"{0:d4}/{1:d2}/{2:d2} {3:d2}:{4:d2}:{5:d2}.{6:d3} 0x{7:X16} 0x{8:X16}\" -f $d.Year, $d.Month, $d.Day, $d.Hour, $d.Minute, $d.Second, $d.Millisecond, $tm, $ft);
for /f "usebackq tokens=1-4 delims= " %%a in (`powershell.exe -Command "%command%"`) do (
	set d=%%a
	set t=%%b
	set tm=%%c
	set ft=%%d
)
set date_yyyy=%d:~0,4%
set date_mm=%d:~5,2%
set date_dd=%d:~8,2%
set time_hh=%t:~0,2%
set time_mm=%t:~3,2%
set time_ss=%t:~6,2%
set time_fff=%t:~9,3%
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
>>%FileName% echo #define __DATETIME_TIME_T__   %tm%
>>%FileName% echo #define __DATETIME_FILETIME__ %ft%
>>%FileName% echo.
>>%FileName% echo #endif	// _DATETIME_H_
endlocal
