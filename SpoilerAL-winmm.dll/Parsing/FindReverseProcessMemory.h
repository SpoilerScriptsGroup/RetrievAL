#ifndef _FINDREVERSEPROCESSMEMORY_H_
#define _FINDREVERSEPROCESSMEMORY_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t __stdcall FindReverseProcessMemoryA(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCSTR  lpString,
	IN          char    c,
	IN          size_t  nMaxLength,
	IN          BOOL    bInsensitiveCase);

size_t __stdcall FindReverseProcessMemoryW(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCWSTR lpString,
	IN          wchar_t c,
	IN          size_t  nMaxLength,
	IN          BOOL    bInsensitiveCase);

#ifdef __cplusplus
}
#endif

#endif	// _FINDREVERSEPROCESSMEMORY_H_
