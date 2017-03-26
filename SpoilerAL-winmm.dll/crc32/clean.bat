@echo off
cd /d "%~dp0"
if exist x86\* (
	if exist x86\*.lib attrib +h x86\*.lib
	del /f /q x86\*
	if exist x86\*.lib attrib -h x86\*.lib
	rmdir /s /q x86\Release
)
if exist x86\Debug\* (
	if exist x86\Debug\*.lib attrib +h x86\Debug\*.lib
	del /f /q x86\Debug\*
	if exist x86\Debug\*.lib attrib -h x86\Debug\*.lib
	for /d %%a in (x86\Debug\*.tlog) do rmdir /s /q %%a
)
if exist x64\* (
	if exist x64\*.lib attrib +h x64\*.lib
	del /f /q x64\*
	if exist x64\*.lib attrib -h x64\*.lib
	rmdir /s /q x64\Release
)
if exist x64\Debug\* (
	if exist x64\Debug\*.lib attrib +h x64\Debug\*.lib
	del /f /q x64\Debug\*
	if exist x64\Debug\*.lib attrib -h x64\Debug\*.lib
	for /d %%a in (x64\Debug\*.tlog) do rmdir /s /q %%a
)
if exist .vs    rmdir /s /q .vs
if exist ipch   rmdir /s /q ipch
if exist *.aps  del /f *.aps
if exist *.bbs  del /f *.bbs
if exist *.clw  del /f *.clw
if exist *.ncb  del /f *.ncb
if exist *.opt  del /f *.opt
if exist *.plg  del /f *.plg
if exist *.sdf  del /f *.sdf
if exist *.suo  attrib -h *.suo
if exist *.suo  del /f *.suo
if exist *.tmp  del /f *.tmp
if exist *.user del /f *.user
