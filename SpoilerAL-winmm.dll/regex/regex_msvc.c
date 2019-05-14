#ifdef __BORLANDC__
#pragma warn -8004
#pragma warn -8008
#pragma warn -8012
#pragma warn -8027
#pragma warn -8066
#endif

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <windows.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include <limits.h>

#ifdef __BORLANDC__
typedef unsigned long mbstate_t;
int __cdecl iswctype(wint_t c, wctype_t desc);
wctype_t __cdecl wctype(const char *property);
#endif

#ifndef MB_LEN_MAX
#define MB_LEN_MAX 5
#endif

#define HAVE_WCTYPE_H 1
#define HAVE_ISWCTYPE 1

#define __USE_GNU
#define __RE_TRANSLATE_TYPE unsigned char *
#define RE_TRANSLATE_TYPE __RE_TRANSLATE_TYPE
#define IS_IN(lib) 1
#define __attribute_warn_unused_result__
#define __attribute(attr)
#define inline __inline

typedef intptr_t ssize_t;

#define __iswlower iswlower
#define __iswalnum iswalnum
#define __towupper towupper
#define __towlower towlower
#define __wcscoll  wcscoll
#define strcasecmp stricmp

#ifdef _DEBUG

#if defined(_MSC_VER) && _MSC_VER < 1400
#define mbsinit(ps) (!(ps) || !*(unsigned long *)(ps))
#endif

#else

#include "regex_msvc.h"

#ifdef _MSC_VER
#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")

#undef isalpha
#undef isupper
#undef islower
#undef isdigit
#undef isxdigit
#undef isspace
#undef ispunct
#undef isblank
#undef isalnum
#undef isprint
#undef isgraph
#undef iscntrl
#undef isascii
#undef tolower
#undef toupper
#undef toascii
#undef iswalpha
#undef iswupper
#undef iswlower
#undef iswdigit
#undef iswxdigit
#undef iswspace
#undef iswpunct
#undef iswblank
#undef iswalnum
#undef iswprint
#undef iswgraph
#undef iswcntrl
#undef iswascii
#undef towlower
#undef towupper
#undef towascii
#define isalpha(c)   _isctype(c, _ALPHA)
#define isupper(c)   _isctype(c, _UPPER)
#define islower(c)   _isctype(c, _LOWER)
#define isdigit(c)   _isctype(c, _DIGIT)
#define isxdigit(c)  _isctype(c, _HEX)
#define isspace(c)   _isctype(c, _SPACE)
#define ispunct(c)   _isctype(c, _PUNCT)
#define isblank(c)   ((c) == '\t' || _isctype(c, _BLANK))
#define isalnum(c)   _isctype(c, _ALPHA | _DIGIT)
#define isprint(c)   _isctype(c, _ALPHA | _DIGIT | _PUNCT | _BLANK)
#define isgraph(c)   _isctype(c, _ALPHA | _DIGIT | _PUNCT)
#define iscntrl(c)   _isctype(c, _CONTROL)
#define isascii(c)   ((unsigned)(c) < 0x80)
#define tolower(c)   (isupper(c) ? (c) + ('a' - 'A') : c)
#define toupper(c)   (islower(c) ? (c) - ('a' - 'A') : c)
#define toascii(c)   ((c) & 0x7F)
#define iswalpha(c)  iswctype(c, _ALPHA)
#define iswupper(c)  iswctype(c, _UPPER)
#define iswlower(c)  iswctype(c, _LOWER)
#define iswdigit(c)  iswctype(c, _DIGIT)
#define iswxdigit(c) iswctype(c, _HEX)
#define iswspace(c)  iswctype(c, _SPACE)
#define iswpunct(c)  iswctype(c, _PUNCT)
#define iswblank(c)  ((c) == L'\t' || iswctype(c, _BLANK))
#define iswalnum(c)  iswctype(c, _ALPHA | _DIGIT)
#define iswprint(c)  iswctype(c, _ALPHA | _DIGIT | _PUNCT | _BLANK)
#define iswgraph(c)  iswctype(c, _ALPHA | _DIGIT | _PUNCT)
#define iswcntrl(c)  iswctype(c, _CONTROL)
#define iswascii(c)  ((unsigned)(c) < 0x80)
#define towascii(c)  ((c) & 0x7F)
#endif

#define malloc inline_malloc
static __inline void *inline_malloc(size_t size)
{
	return HeapAlloc(HEAP_HANDLE, 0, size ? size : 1);
}

#define calloc inline_calloc
static __inline void *inline_calloc(size_t num, size_t size)
{
	return HeapAlloc(HEAP_HANDLE, HEAP_ZERO_MEMORY, (size *= num) ? size : 1);
}

#define realloc inline_realloc
static __inline void *inline_realloc(void *memblock, size_t size)
{
	if (size)
		return memblock ?
			HeapReAlloc(HEAP_HANDLE, 0, memblock, size) :
			HeapAlloc(HEAP_HANDLE, 0, size);
	HeapFree(HEAP_HANDLE, 0, memblock);
	return NULL;
}

#define free(memblock) HeapFree(HEAP_HANDLE, 0, memblock)

#if !HAVE_MEMCPY
#define memcpy memmove
#endif

#if !HAVE_MEMMOVE
#define memmove inline_memmove
#undef RtlMoveMemory
EXTERN_C __declspec(dllimport) void WINAPI RtlMoveMemory(void *Destination, const void *Source, size_t Length);
static __inline void *inline_memmove(void *dest, const void *src, size_t count)
{
	RtlMoveMemory(dest, src, count);
	return dest;
}
#endif

#if !HAVE_MEMSET
#define memset inline_memset
#undef RtlFillMemory
EXTERN_C __declspec(dllimport) void WINAPI RtlFillMemory(void *Destination, size_t Length, UCHAR Fill);
static __inline void *inline_memset(void *dest, int c, size_t count)
{
	RtlFillMemory(dest, count, c);
	return dest;
}
#endif

#if !HAVE_ABORT
#define abort()                                                           \
do {                                                                      \
    MessageBoxA(NULL, "Abnormal program termination", NULL, MB_ICONSTOP); \
    ExitProcess(3);                                                       \
} while (0)
#endif

#ifdef __BORLANDC__
#undef VARIABLE_LOCALE
#endif

#if !VARIABLE_LOCALE
#define LOCALE_ID GetThreadLocale()
#define CODE_PAGE CP_THREAD_ACP

#ifndef __BORLANDC__
#undef MB_CUR_MAX
#define MB_CUR_MAX ___mb_cur_max_func()
static __inline int ___mb_cur_max_func()
{
	CPINFO cpinfo;
	return GetCPInfo(CODE_PAGE, &cpinfo) ? cpinfo.MaxCharSize : 0;
}
#endif
#else
#define LOCALE_ID get_regex_lcid()
#define CODE_PAGE regex_codepage

static LCID regex_lcid = 0;
static UINT regex_codepage = CP_THREAD_ACP;
static int  regex_mb_cur_max = 0;

LCID get_regex_lcid()
{
	if (!regex_lcid)
		regex_lcid = GetThreadLocale();
	return regex_lcid;
}

LCID set_regex_lcid(LCID locale)
{
	string s[16];

	if (!GetLocaleInfoA(locale, LOCALE_IDEFAULTANSICODEPAGE, s, sizeof(s)))
	{
		UINT   codepage;
		CPINFO cpinfo;

		codepage = strtoul(s, NULL, 10);
		if (GetCPInfo(codepage, &cpinfo))
		{
			regex_lcid = locale;
			regex_codepage = codepage;
			regex_mb_cur_max = cpinfo.MaxCharSize;
			return locale;
		}
	}
	return 0;
}

#undef MB_CUR_MAX
#define MB_CUR_MAX ___mb_cur_max_func()
static __inline int ___mb_cur_max_func()
{
	CPINFO cpinfo;

	if (!regex_mb_cur_max)
		if (GetCPInfo(CODE_PAGE, &cpinfo))
			regex_mb_cur_max = cpinfo.MaxCharSize;
	return regex_mb_cur_max;
}
#endif

#define HAVE_LANGINFO_CODESET 1
#define nl_langinfo(item) (CODE_PAGE != CP_UTF8 ? "" : "UTF-8")

#ifdef _MSC_VER
static __inline wint_t towupper(wint_t c)
{
	wchar_t dest;
	return LCMapStringW(LOCALE_ID != CP_THREAD_ACP ? LOCALE_ID : GetThreadLocale(), LCMAP_UPPERCASE, (LPCWSTR)&c, 1, (LPWSTR)&dest, 1) ?
		dest :
		c;
}
#endif

#ifdef _MSC_VER
static __inline wint_t towlower(wint_t c)
{
	wchar_t dest;
	return LCMapStringW(LOCALE_ID != CP_THREAD_ACP ? LOCALE_ID : GetThreadLocale(), LCMAP_LOWERCASE, (LPCWSTR)&c, 1, (LPWSTR)&dest, 1) ?
		dest :
		c;
}
#endif

#ifdef _MSC_VER
static __inline int wcscoll(const wchar_t *string1, const wchar_t *string2)
{
	int ret;
	return (ret = CompareStringW(LOCALE_ID != CP_THREAD_ACP ? LOCALE_ID : GetThreadLocale(), SORT_STRINGSORT, string1, -1, string2, -1)) ?
		ret - CSTR_EQUAL :
		_NLSCMPERROR;
}
#endif

#define btowc(c) ((c) != EOF ? (wint_t)(unsigned char)(c) : WEOF)

#define mbsinit(ps) (!(ps) || !*(unsigned long *)(ps))

static __inline size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
	if (!s || !n || !*s)
		return 0;
	if (!*(unsigned long *)ps)
	{
		if (!IsDBCSLeadByteEx(CODE_PAGE, *s))
		{
			if (MultiByteToWideChar(CODE_PAGE, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, s, 1, pwc, !!pwc))
				return 1;
		}
		else if (n >= 2)
		{
			if (MultiByteToWideChar(CODE_PAGE, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, s, 2, pwc, !!pwc))
				return 2;
		}
		else
		{
			((char *)ps)[0] = *s;
			return -2;
		}
	}
	else
	{
		((char *)ps)[1] = *s;
		if (MultiByteToWideChar(CODE_PAGE, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, (char *)ps, 2, pwc, !!pwc))
		{
			*(unsigned long *)ps = 0;
			return 1;
		}
	}
	*(unsigned long *)ps = 0;
	if (pwc)
		*pwc = L'\0';
	return -1;
}

static __inline size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps)
{
	char lpBuffer[MB_LEN_MAX];
	BOOL bUsedDefaultChar;
	int  cbMultiByte;

	if (ps)
		*(unsigned long *)ps = 0;
	if (!s)
		s = lpBuffer;
	cbMultiByte = WideCharToMultiByte(CODE_PAGE, 0, &wc, 1, s, MB_LEN_MAX, NULL, &bUsedDefaultChar);
	return (cbMultiByte && !bUsedDefaultChar) ? cbMultiByte : -1;
}

#endif	// _DEGUG

#pragma warning(disable: 4018 4244)
#include "regex.c"

