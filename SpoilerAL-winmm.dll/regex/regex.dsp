# Microsoft Developer Studio Project File - Name="regex" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=regex - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "regex.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "regex.mak" CFG="regex - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "regex - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "regex - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "regex - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "c99" /I "include" /I "posix" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"lib/x86/regex.pch" /YX /Fo"lib/x86/" /Fd"lib/x86/" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"lib/x86/regex.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\x86\regex.lib"

!ELSEIF  "$(CFG)" == "regex - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "c99" /I "include" /I "posix" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"lib/x86/Debug/regex.pch" /YX /Fo"lib/x86/Debug/" /Fd"lib/x86/Debug/" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"lib/x86/Debug/regex.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\x86\Debug\regex.lib"

!ENDIF 

# Begin Target

# Name "regex - Win32 Release"
# Name "regex - Win32 Debug"
# Begin Group "crt"

# PROP Default_Filter ""
# Begin Group "x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\crt\x86\alloca16.asm

!IF  "$(CFG)" == "regex - Win32 Release"

!ELSEIF  "$(CFG)" == "regex - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\crt\x86\chkstk.asm

!IF  "$(CFG)" == "regex - Win32 Release"

!ELSEIF  "$(CFG)" == "regex - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\crt\x86\vcruntime.inc

!IF  "$(CFG)" == "regex - Win32 Release"

!ELSEIF  "$(CFG)" == "regex - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\crt\isctype.c

!IF  "$(CFG)" == "regex - Win32 Release"

!ELSEIF  "$(CFG)" == "regex - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\crt\iswctype.c

!IF  "$(CFG)" == "regex - Win32 Release"

!ELSEIF  "$(CFG)" == "regex - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\crt\wctype.c

!IF  "$(CFG)" == "regex - Win32 Release"

!ELSEIF  "$(CFG)" == "regex - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Group "sys"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\sys\param.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\alloca.h
# End Source File
# Begin Source File

SOURCE=.\include\langinfo.h
# End Source File
# End Group
# Begin Group "posix"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\posix\regcomp.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\posix\regex.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\posix\regex.h
# End Source File
# Begin Source File

SOURCE=.\posix\regex_internal.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\posix\regex_internal.h
# End Source File
# Begin Source File

SOURCE=.\posix\regexec.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "c99"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\c99\stdbool.h
# End Source File
# Begin Source File

SOURCE=.\c99\stdint.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\regex.h
# End Source File
# Begin Source File

SOURCE=.\regex_msvc.c
# End Source File
# Begin Source File

SOURCE=.\regex_msvc.h
# End Source File
# End Target
# End Project
