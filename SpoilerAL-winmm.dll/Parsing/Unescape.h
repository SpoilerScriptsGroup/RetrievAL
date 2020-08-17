#ifndef _UNESCAPE_H_
#define _UNESCAPE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN64
char * __fastcall UnescapeA(char *first, char **plast, BOOL breakSingleQuate);
wchar_t * __fastcall UnescapeW(wchar_t *first, wchar_t **plast, BOOL breakSingleQuate);
unsigned char * __fastcall UnescapeU(unsigned char *first, unsigned char **plast, BOOL breakSingleQuate);
#else
unsigned __int64 __fastcall __reg64return_UnescapeA(char *first, char *last, BOOL breakSingleQuate);
static __forceinline char *UnescapeA(char *first, char **plast, BOOL breakSingleQuate)
{
	unsigned __int64 result;

	result = __reg64return_UnescapeA(first, *plast, breakSingleQuate);
	*plast = (char *)(result >> 32);
	return (char *)result;
}
unsigned __int64 __fastcall __reg64return_UnescapeW(wchar_t *first, wchar_t *last, BOOL breakSingleQuate);
static __forceinline wchar_t *UnescapeW(wchar_t *first, wchar_t **plast, BOOL breakSingleQuate)
{
	unsigned __int64 result;

	result = __reg64return_UnescapeW(first, *plast, breakSingleQuate);
	*plast = (wchar_t *)(result >> 32);
	return (wchar_t *)result;
}
unsigned __int64 __fastcall __reg64return_UnescapeU(unsigned char *first, unsigned char *last, BOOL breakSingleQuate);
static __forceinline unsigned char *UnescapeU(unsigned char *first, unsigned char **plast, BOOL breakSingleQuate)
{
	unsigned __int64 result;

	result = __reg64return_UnescapeU(first, *plast, breakSingleQuate);
	*plast = (unsigned char *)(result >> 32);
	return (unsigned char *)result;
}
#endif
__int64 __fastcall UnescapeAnsiCharA(const char **pfirst, const char *last);
unsigned long __fastcall UnescapeUnicodeCharA(const char **pfirst, const char *last);
unsigned long __fastcall UnescapeUtf8CharA(const char **pfirst, const char *last);

#ifdef __cplusplus
}
#endif

#endif	// _UNESCAPE_H_
