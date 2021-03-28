@echo off

setlocal EnableDelayedExpansion
if "%~1" == "" (
	set FileName=export64.asm
) else (
	set FileName=%1
)
echo Creating %FileName%...
> %FileName% echo public ExportAddresses
for /f "tokens=1,2 delims=(," %%a in (export.h) do (
	set name=%%b
	if defined name (
		set noname=%%a
		set noname=!noname: =!
		if "!noname!" equ "NAMED" (
			set name=!name: =!
			if defined name (
				>>%FileName% echo public _imp_!name!
			)
		)
	)
)
for /f "tokens=2 delims=(," %%a in (export.h) do (
	set name=%%a
	set name=!name: =!
	>>%FileName% echo public _exp_!name!
)
>>%FileName% echo.
>>%FileName% echo .data
>>%FileName% echo.
>>%FileName% echo align 8
>>%FileName% echo ExportAddresses label qword
set len=0
for /f "tokens=2 delims=(," %%a in (export.h) do (
	set name=%%a
	set name=!name: =!
	call :strlen i name
	if !len! lss !i! (
		set len=!i!
	)
)
set pad=
for /l %%i in (1,1,%len%) do (
	set pad= !pad!
)
for /f "tokens=2 delims=(," %%a in (export.h) do (
	set name=%%a
	set name=!name: =!
	set name=!name!!pad!
	set name=!name:~0,%len%!
	>>%FileName% echo _imp_!name! qword ?
)
>>%FileName% echo.
>>%FileName% echo .code
for /f "tokens=2 delims=(," %%a in (export.h) do (
	set name=%%a
	set name=!name: =!
	>>%FileName% echo.
	>>%FileName% echo align 16
	>>%FileName% echo _exp_!name!:
	>>%FileName% echo 	jmp     qword ptr [_imp_!name!]
)
>>%FileName% echo.
>>%FileName% echo end
endlocal
goto :eof

:strlen <result> <string>
(
	setlocal EnableDelayedExpansion
	set tmp=!%~2!
	if defined tmp (
		set len=1
		for %%i in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
			if not "!tmp:~%%i,1!" == "" (
				set /a len+=%%i
				set tmp=!tmp:~%%i!
			)
		)
	) else (
		set len=0
	)
)
(
	endlocal
	set %~1=%len%
	exit /b
)
