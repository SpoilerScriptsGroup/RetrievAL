#ifndef _FILLPROCESSMEMORY_H_
#define _FILLPROCESSMEMORY_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall FillProcessMemory(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          BYTE   bFill);

BOOL __stdcall FillProcessMemory16(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          WORD   wFill);

BOOL __stdcall FillProcessMemory32(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          DWORD  dwFill);

BOOL __stdcall FillProcessMemory64(
	IN OPTIONAL HANDLE           hProcess,
	IN          PVOID            lpDest,
	IN          size_t           nCount,
	IN          unsigned __int64 qwFill);

#ifdef __cplusplus
}
#endif

#endif	// _FILLPROCESSMEMORY_H_
