#ifndef _FINDPROCESSMEMORY_H_
#define _FINDPROCESSMEMORY_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t __stdcall FindProcessMemoryA(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCSTR  lpString,
	IN          char    c,
	IN          size_t  nMaxLength);

size_t __stdcall FindProcessMemoryW(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCWSTR lpString,
	IN          wchar_t c,
	IN          size_t  nMaxLength);

#ifdef __cplusplus
}
#endif

#define StringLengthA(hProcess, lpString, nMaxLength) \
	FindProcessMemoryA(hProcess, lpString, '\0', nMaxLength)

#define StringLengthW(hProcess, lpString, nMaxLength) \
	FindProcessMemoryW(hProcess, lpString, L'\0', nMaxLength)

#endif	// _FINDPROCESSMEMORY_H_
