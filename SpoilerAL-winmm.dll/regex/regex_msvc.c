#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>

#define __USE_GNU
#define __RE_TRANSLATE_TYPE unsigned char *
#define RE_TRANSLATE_TYPE __RE_TRANSLATE_TYPE
#define IS_IN(lib) 1
#define __attribute_warn_unused_result__
#define __attribute(attr)

#define strcasecmp _stricmp

#pragma warning(disable: 4018 4244)

#ifndef _DEBUG

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")

#include <windows.h>
#include <winnls.h>
#include "regex_msvc.h"

#define malloc(size)            HeapAlloc(HEAP_HANDLE, 0, (size_t)(size))
#define calloc(num, size)       HeapAlloc(HEAP_HANDLE, HEAP_ZERO_MEMORY, (size_t)(num) * (size_t)(size))
#define realloc(memblock, size) ((memblock) ? HeapReAlloc(HEAP_HANDLE, 0, memblock, size) : malloc(size))
#define free(memblock)          HeapFree(HEAP_HANDLE, 0, memblock)

#define abort()                                                                        \
do {                                                                                   \
    MessageBoxA(NULL, "abnormal program termination", NULL, MB_ICONSTOP | MB_TOPMOST); \
    ExitProcess(3);                                                                    \
} while (0)

#if !HAVE_MEMMOVE || !HAVE_MEMCPY
#ifdef RtlMoveMemory
#undef RtlMoveMemory
#endif
EXTERN_C __declspec(dllimport) void WINAPI RtlMoveMemory(void *Destination, const void *Source, size_t Length);
#endif

#if !HAVE_MEMCPY
#define memcpy RtlMoveMemory
#endif

#if !HAVE_MEMMOVE
#define memmove RtlMoveMemory
#endif

#if !HAVE_MEMSET
#ifdef RtlFillMemory
#undef RtlFillMemory
#endif
EXTERN_C __declspec(dllimport) void WINAPI RtlFillMemory(void *Destination, size_t Length, UCHAR Fill);
#define memset(dest, c, count) RtlFillMemory(dest, count, c)
#endif

#if !HAVE_STRICMP
#define _stricmp lstrcmpiA
#endif

#if !HAVE_STRCHR
__forceinline char *strchr(const char *str, int c)
{
	while (*str != (char)c)
	{
		if (!*(++str))
		{
			str = NULL;
			break;
		}
	}
	return (char *)str;
}
#endif

#if !HAVE_GETENV
__forceinline char *getenv(const char *varname)
{
	static char buffer[1024];
	return GetEnvironmentVariableA(varname, buffer, _countof(buffer)) ? buffer : NULL;
}
#endif

#if !HAVE_MB_CUR_MAX
#ifdef MB_CUR_MAX
#undef MB_CUR_MAX
#endif
#define MB_CUR_MAX ____mb_cur_max_func()
__forceinline size_t ____mb_cur_max_func()
{
	switch (GetThreadLocale())
	{
	case MAKELCID(MAKELANGID(LANG_CHINESE , SUBLANG_CHINESE_TRADITIONAL), SORT_DEFAULT):
	case MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN     ), SORT_DEFAULT):
	case MAKELCID(MAKELANGID(LANG_KOREAN  , SUBLANG_KOREAN             ), SORT_DEFAULT):
	case MAKELCID(MAKELANGID(LANG_CHINESE , SUBLANG_CHINESE_SIMPLIFIED ), SORT_DEFAULT):
	case MAKELCID(MAKELANGID(LANG_CHINESE , SUBLANG_CHINESE_HONGKONG   ), SORT_DEFAULT):
	case MAKELCID(MAKELANGID(LANG_CHINESE , SUBLANG_CHINESE_SINGAPORE  ), SORT_DEFAULT):
		return 2;
	}
	return 1;
}
#endif

#endif	// _DEGUG

#include "regex.c"
