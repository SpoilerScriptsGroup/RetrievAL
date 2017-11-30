# Microsoft Developer Studio Project File - Name="EditBinEx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=EditBinEx - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "EditBinEx.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "EditBinEx.mak" CFG="EditBinEx - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "EditBinEx - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "EditBinEx - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EditBinEx - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "EditBinEx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "EditBinEx - Win32 Release"
# Name "EditBinEx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EditBinEx.c
# End Source File
# Begin Source File

SOURCE=.\Internal.c
# End Source File
# Begin Source File

SOURCE=.\RemoveDebugSection.c
# End Source File
# Begin Source File

SOURCE=.\SetDelayImport.c
# End Source File
# Begin Source File

SOURCE=.\SetLastWriteTime.c
# End Source File
# Begin Source File

SOURCE=.\SetLinkerVersion.c
# End Source File
# Begin Source File

SOURCE=.\SetOperatingSystemVersion.c
# End Source File
# Begin Source File

SOURCE=.\SetTimeDateStamp.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Internal.h
# End Source File
# Begin Source File

SOURCE=.\RemoveDebugSection.h
# End Source File
# Begin Source File

SOURCE=.\SetDelayImport.h
# End Source File
# Begin Source File

SOURCE=.\SetLastWriteTime.h
# End Source File
# Begin Source File

SOURCE=.\SetLinkerVersion.h
# End Source File
# Begin Source File

SOURCE=.\SetOperatingSystemVersion.h
# End Source File
# Begin Source File

SOURCE=.\SetTimeDateStamp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "crt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\crt\_wcslen.h
# End Source File
# Begin Source File

SOURCE=.\crt\memset.h
# End Source File
# Begin Source File

SOURCE=.\crt\ParseArgument.h
# End Source File
# Begin Source File

SOURCE=.\crt\ParseArgumentA.c
# End Source File
# Begin Source File

SOURCE=.\crt\ParseArgumentT.h
# End Source File
# Begin Source File

SOURCE=.\crt\ParseArgumentW.c
# End Source File
# Begin Source File

SOURCE=.\crt\wassert.c
# End Source File
# Begin Source File

SOURCE=.\crt\wcslen.h
# End Source File
# Begin Source File

SOURCE=.\crt\wcsnicmp.c
# End Source File
# Begin Source File

SOURCE=.\crt\wmemcpy.h
# End Source File
# Begin Source File

SOURCE=.\crt\wmemmove.h
# End Source File
# End Group
# Begin Group "IsBadPtr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IsBadPtr\IsBadPtr.asm
# End Source File
# Begin Source File

SOURCE=.\IsBadPtr\IsBadPtrSource.h
# End Source File
# Begin Source File

SOURCE=.\IsBadPtr\IsBadReadPtr.c
# End Source File
# Begin Source File

SOURCE=.\IsBadPtr\IsBadWritePtr.c
# End Source File
# End Group
# End Target
# End Project
