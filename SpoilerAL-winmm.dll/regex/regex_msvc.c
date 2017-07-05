#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>

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
#define __iswalnum iswalnum
#define __towupper towupper
#define __towlower towlower
#define __wcscoll  wcscoll
#define strcasecmp _stricmp

#ifndef _DEBUG

#include "regex_msvc.h"

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")

#pragma warning(push)
#pragma warning(disable:4005)
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
#undef  towlower
#undef  towupper
#define towascii(c)  ((c) & 0x7F)
#pragma warning(pop)

#define malloc(size)            HeapAlloc(HEAP_HANDLE, 0, (size_t)(size))
#define calloc(num, size)       HeapAlloc(HEAP_HANDLE, HEAP_ZERO_MEMORY, (size_t)(num) * (size_t)(size))
#define realloc(memblock, size) ((memblock) ? HeapReAlloc(HEAP_HANDLE, 0, memblock, size) : malloc(size))
#define free(memblock)          HeapFree(HEAP_HANDLE, 0, memblock)

#if !HAVE_MEMMOVE || !HAVE_MEMCPY
#undef RtlMoveMemory
EXTERN_C __declspec(dllimport) void WINAPI RtlMoveMemory(void *Destination, const void *Source, size_t Length);
#endif

#if !HAVE_MEMCPY
#define memcpy RtlMoveMemory
#endif

#if !HAVE_MEMMOVE
#define memmove RtlMoveMemory
#endif

#if !HAVE_MEMSET
#undef RtlFillMemory
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


#if !VARIABLE_LOCALE
#define LOCALE_ID GetThreadLocale()
#define CODE_PAGE CP_THREAD_ACP

#undef MB_CUR_MAX
#define MB_CUR_MAX ___mb_cur_max_func()
static __inline int ___mb_cur_max_func()
{
	CPINFO cpinfo;
	return GetCPInfo(CODE_PAGE, &cpinfo) ? cpinfo.MaxCharSize : 0;
}
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
	UINT   codepage;
	CPINFO cpinfo;

	if (GetLocaleInfoA(locale, LOCALE_IDEFAULTANSICODEPAGE, (LPSTR)&codepage, sizeof(codepage)) &&
		GetCPInfo(codepage, &cpinfo))
	{
		regex_lcid = locale;
		regex_codepage = codepage;
		regex_mb_cur_max = cpinfo.MaxCharSize;
		return locale;
	}
	else
	{
		return 0;
	}
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

static __inline wint_t towupper(wint_t c)
{
	wchar_t dest;
	return LCMapStringW(LOCALE_ID, LCMAP_UPPERCASE, (LPCWSTR)&c, 1, (LPWSTR)&dest, 1) ?
		dest :
		c;
}

static __inline wint_t towlower(wint_t c)
{
	wchar_t dest;
	return LCMapStringW(LOCALE_ID, LCMAP_LOWERCASE, (LPCWSTR)&c, 1, (LPWSTR)&dest, 1) ?
		dest :
		c;
}

static __inline int wcscoll(const wchar_t *string1, const wchar_t *string2)
{
	int ret;
	return (ret = CompareStringW(LOCALE_ID, SORT_STRINGSORT, string1, -1, string2, -1)) ?
		ret - CSTR_EQUAL :
		_NLSCMPERROR;
}

#define btowc(c) ((c) != EOF ? (wint_t)(unsigned char)(c) : WEOF)

#define mbsinit(ps) (!(ps) || !(ps)->_Wchar)

static __inline size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
	if (!s || !n || !*s)
		return 0;
	if (!ps->_Wchar)
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
			((char *)ps->_Wchar)[0] = *s;
			return -2;
		}
	}
	else
	{
		((char *)ps->_Wchar)[1] = *s;
		if (MultiByteToWideChar(CODE_PAGE, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, (char *)ps->_Wchar, 2, pwc, !!pwc))
		{
			ps->_Wchar = 0;
			return 1;
		}
	}
	ps->_Wchar = 0;
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
		ps->_Wchar = 0;
	if (!s)
		s = lpBuffer;
	cbMultiByte = WideCharToMultiByte(CODE_PAGE, 0, &wc, 1, s, MB_LEN_MAX, NULL, &bUsedDefaultChar);
	return (cbMultiByte && !bUsedDefaultChar) ? cbMultiByte : -1;
}

#endif	// _DEGUG

#pragma warning(disable: 4018 4244)
#include "regex.c"
