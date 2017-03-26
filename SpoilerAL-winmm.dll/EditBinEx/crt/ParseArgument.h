#ifndef _PARSEARGUMENT_H_
#define _PARSEARGUMENT_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

char ** __stdcall ParseArgumentA(HANDLE hHeap, const char *lpParameters, int *argc);
wchar_t ** __stdcall ParseArgumentW(HANDLE hHeap, const wchar_t *lpParameters, int *argc);

#ifdef _UNICODE
#define ParseArgument ParseArgumentW
#else
#define ParseArgument ParseArgumentA
#endif

#ifdef __cplusplus
}
#endif

#endif	// _PARSEARGUMENT_H_
