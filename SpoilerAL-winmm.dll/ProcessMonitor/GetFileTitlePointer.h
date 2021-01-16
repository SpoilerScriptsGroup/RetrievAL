#ifndef _GETFILETITLEPOINTER_H_
#define _GETFILETITLEPOINTER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 1
#include <Shlwapi.h>
#define GetFileTitlePointerA PathFindFileNameA
#define GetFileTitlePointerW PathFindFileNameW
#else
char * __fastcall GetFileTitlePointerA(const char *lpFileNeme);
wchar_t * __fastcall GetFileTitlePointerW(const wchar_t *lpFileNeme);
#endif

#ifdef __cplusplus
}
#endif

#endif	// _GETFILETITLEPOINTER_H_
