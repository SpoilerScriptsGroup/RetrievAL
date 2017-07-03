#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>

#pragma warning(push)
#pragma warning(disable:4005)
#define isalpha(c)   _isctype(c, _UPPER | _LOWER)
#define isupper(c)   _isctype(c, _UPPER)
#define islower(c)   _isctype(c, _LOWER)
#define isdigit(c)   _isctype(c, _DIGIT)
#define isxdigit(c)  _isctype(c, _HEX)
#define isspace(c)   _isctype(c, _SPACE)
#define ispunct(c)   _isctype(c, _PUNCT)
#define isblank(c)   ((c) == ' ' || (c) == '\t')
#define isalnum(c)   _isctype(c, _UPPER | _LOWER | _DIGIT)
#define isprint(c)   _isctype(c, _UPPER | _LOWER | _DIGIT | _PUNCT | _BLANK)
#define isgraph(c)   _isctype(c, _UPPER | _LOWER | _DIGIT | _PUNCT)
#define iscntrl(c)   _isctype(c, _CONTROL)
#define isascii(c)   ((unsigned)(c) < 0x80)
#define tolower(c)   (isupper(c) ? (c) + ('a' - 'A') : c)
#define toupper(c)   (islower(c) ? (c) - ('a' - 'A') : c)
#define toascii(c)   ((c) & 0x7F)
#define iswalpha(c)  iswctype(c, _UPPER | _LOWER)
#define iswupper(c)  iswctype(c, _UPPER)
#define iswlower(c)  iswctype(c, _LOWER)
#define iswdigit(c)  iswctype(c, _DIGIT)
#define iswxdigit(c) iswctype(c, _HEX)
#define iswspace(c)  iswctype(c, _SPACE)
#define iswpunct(c)  iswctype(c, _PUNCT)
#define iswblank(c)  ((c) == L' ' || (c) == L'\t')
#define iswalnum(c)  iswctype(c, _UPPER | _LOWER | _DIGIT)
#define iswprint(c)  iswctype(c, _UPPER | _LOWER | _DIGIT | _PUNCT | _BLANK)
#define iswgraph(c)  iswctype(c, _UPPER | _LOWER | _DIGIT | _PUNCT)
#define iswcntrl(c)  iswctype(c, _CONTROL)
#define iswascii(c)  ((unsigned)(c) < 0x80)
#define towlower(c)  (iswupper(c) ? (c) + (L'a' - L'A') : c)
#define towupper(c)  (iswlower(c) ? (c) - (L'a' - L'A') : c)
#define towascii(c)  ((c) & 0x7F)
#pragma warning(pop)

#define HAVE_WCTYPE_H 1
#define HAVE_ISWCTYPE 1

#define __USE_GNU
#define __RE_TRANSLATE_TYPE unsigned char *
#define RE_TRANSLATE_TYPE __RE_TRANSLATE_TYPE
#define IS_IN(lib) 1
#define __attribute_warn_unused_result__
#define __attribute(attr)

typedef intptr_t ssize_t;

#define __iswlower iswlower
#define __towlower towlower
#define __towupper towupper
#define __iswalnum iswalnum
#define __wcscoll  wcscmp
#define strcasecmp _stricmp
#define btowc(c)   ((c) != EOF ? (wint_t)(unsigned char)(c) : WEOF)

#ifndef _DEBUG

#include "regex_msvc.h"

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")

#define HAVE_LANGINFO_CODESET 1

#define CodePage CP_ACP

#define malloc(size)            HeapAlloc(HEAP_HANDLE, 0, (size_t)(size))
#define calloc(num, size)       HeapAlloc(HEAP_HANDLE, HEAP_ZERO_MEMORY, (size_t)(num) * (size_t)(size))
#define realloc(memblock, size) ((memblock) ? HeapReAlloc(HEAP_HANDLE, 0, memblock, size) : malloc(size))
#define free(memblock)          HeapFree(HEAP_HANDLE, 0, memblock)

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

#if !HAVE_ABORT
#define abort()                                                                        \
do {                                                                                   \
    MessageBoxA(NULL, "abnormal program termination", NULL, MB_ICONSTOP | MB_TOPMOST); \
    ExitProcess(3);                                                                    \
} while (0)
#endif

#if !HAVE_MB_CUR_MAX
#ifdef MB_CUR_MAX
#undef MB_CUR_MAX
#endif
#define MB_CUR_MAX ___mb_cur_max_func()
__forceinline int ___mb_cur_max_func()
{
	CPINFO cpinfo;
	return GetCPInfo(CodePage, &cpinfo) ? cpinfo.MaxCharSize : 0;
}
#endif

#define nl_langinfo(item) (CodePage != CP_UTF8 ? "" : "UTF-8")

#define mbsinit(ps) !(ps)->_State

__forceinline size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
	ps->_State = 1;
	int cchWideChar = MultiByteToWideChar(CodePage, MB_ERR_INVALID_CHARS, s, n, pwc, -1);
	return
		cchWideChar ?
			pwc[cchWideChar - 1] == L'\0' ?
				ps->_State = 0 :
				WideCharToMultiByte(CodePage, 0, pwc, cchWideChar, NULL, 0, NULL, NULL) :
			GetLastError() == ERROR_NO_UNICODE_TRANSLATION ?
				(intptr_t)-1 :
				(intptr_t)-2;
}

__forceinline size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps)
{
	ps->_State = 1;
	int cchMultiByte = WideCharToMultiByte(CodePage, 0, &wc, 1, s, MB_LEN_MAX, NULL, NULL);
	return cchMultiByte ? cchMultiByte : (intptr_t)-1;
}

#endif	// _DEGUG

#pragma warning(disable: 4018 4244)
#include "regex.c"
