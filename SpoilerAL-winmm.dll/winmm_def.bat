@echo off

setlocal EnableDelayedExpansion
if not defined inProcessing (
	set inProcessing=1
	echo Creating winmm.def...
	%0 >winmm.def
	endlocal
	goto :eof
)
echo LIBRARY winmm
echo EXPORTS
set exportNameLen=0
set functionNameLen=0
set ordinalLen=0
for /f "tokens=1,2,4 delims=(," %%a in (export.h) do (
	set name=%%b
	if defined name (
		set name=!name: =!
		call :strlen len name
		set noname=%%a
		set noname=!noname: =!
		if "!noname!" equ "NAMED" (
			if !exportNameLen! lss !len! (
				set exportNameLen=!len!
			)
		) else (
			if !functionNameLen! lss !len! (
				set functionNameLen=!len!
			)
		)
		set ordinal=%%c
		set ordinal=!ordinal: =!
		call :strlen len ordinal
		if !ordinalLen! lss !len! (
			set ordinalLen=!len!
		)
	)
)
if !functionNameLen! lss !exportNameLen! (
	set functionNameLen=!exportNameLen!
)
set /a functionNameLen+=5
set exportNamePad=
for /l %%i in (1,1,%exportNameLen%) do (
	set exportNamePad= !exportNamePad!
)
set ordinalPad=
for /l %%i in (1,1,%ordinalLen%) do (
	set ordinalPad= !ordinalPad!
)
for /f "tokens=1,2,4 delims=(," %%a in (export.h) do (
	set exportName=%%b
	if defined exportName (
		set exportName=!exportName: =!
		set functionName=_exp_!exportName!!exportNamePad!
		set functionName=!functionName:~0,%functionNameLen%!
		set ordinal=%%c
		set ordinal=!ordinal: =!
		set ordinal=!ordinalPad!!ordinal!
		set ordinal=!ordinal:~-%ordinalLen%!
		set noname=%%a
		set noname=!noname: =!
		if "!noname!" equ "NAMED" (
			set exportName=!exportName!!exportNamePad!
			set functionName== !functionName!
			set noname=
		) else (
			set exportName=!exportNamePad!
			set functionName=  !functionName!
			set noname= NONAME
		)
		set exportName=!exportName:~0,%exportNameLen%!
		echo 	!exportName! !functionName! @ !ordinal!!noname!
	)
)
endlocal
goto :eof

:strlen <result> <string>
(
	setlocal EnableDelayedExpansion
	set tmp=!%~2!
	if defined tmp (
		set len=1
		for %%i in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
			if "!tmp:~%%i,1!" neq "" (
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
