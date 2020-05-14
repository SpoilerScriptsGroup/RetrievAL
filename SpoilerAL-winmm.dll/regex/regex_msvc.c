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
#if SIZE_MAX < UINT_MAX	/* bug fix */
#undef SIZE_MAX
#define SIZE_MAX UINT_MAX
#endif
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
#define __GNUC_PREREQ(major, minor) 0
#define inline __inline
#define __attribute__(attr)
#define __attribute_warn_unused_result__
#define __glibc_unlikely
#define __glibc_likely
#define assume __assume
#define weak_alias(name, aliasname)
#define libc_hidden_def(name)

typedef intptr_t ssize_t;

#define __iswalnum iswalnum
#define __towupper towupper
#define __towlower towlower
#define __wcscoll  wcscoll

#ifdef _DEBUG

#if defined(_MSC_VER) && _MSC_VER < 1400
#define mbsinit(ps) (!(ps) || !*(unsigned long *)(ps))
#endif

#define LOCALE_ID GetThreadLocale()

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
#define isalpha   inline_isalpha
#define isupper   inline_isupper
#define islower   inline_islower
#define isdigit   inline_isdigit
#define isxdigit  inline_isxdigit
#define isspace   inline_isspace
#define ispunct   inline_ispunct
#define isblank   inline_isblank
#define isalnum   inline_isalnum
#define isprint   inline_isprint
#define isgraph   inline_isgraph
#define iscntrl   inline_iscntrl
#define isascii   inline_isascii
#define tolower   inline_tolower
#define toupper   inline_toupper
#define toascii   inline_toascii
#define iswalpha  inline_iswalpha
#define iswupper  inline_iswupper
#define iswlower  inline_iswlower
#define iswdigit  inline_iswdigit
#define iswxdigit inline_iswxdigit
#define iswspace  inline_iswspace
#define iswpunct  inline_iswpunct
#define iswblank  inline_iswblank
#define iswalnum  inline_iswalnum
#define iswprint  inline_iswprint
#define iswgraph  inline_iswgraph
#define iswcntrl  inline_iswcntrl
#define iswascii  inline_iswascii
#define towlower  inline_towlower
#define towupper  inline_towupper
#define towascii  inline_towascii
static __inline int inline_isalpha(int c)        { return (unsigned int)((c | ('a' - 'A')) - 'a') < 'z' - 'a' + 1; }
static __inline int inline_isupper(int c)        { return (unsigned int)(c - 'A') < 'Z' - 'A' + 1; }
static __inline int inline_islower(int c)        { return (unsigned int)(c - 'a') < 'z' - 'a' + 1; }
static __inline int inline_isdigit(int c)        { return (unsigned int)(c - '0') < '9' - '0' + 1; }
static __inline int inline_isxdigit(int c)       { unsigned int x = c; c |= 'a' - 'A'; x -= '0'; c -= 'a'; return x < '9' - '0' + 1 || (unsigned int)c < 'f' - 'a' + 1; }
static __inline int inline_isspace(int c)        { return c == ' ' || (unsigned int)(c - '\t') < '\r' - '\t' + 1; }
static __inline int inline_ispunct(int c)        { unsigned int x = c; c |= 'a' - 'A'; x -= 0x21; c -= 'z' + 1; return x < 0x7E - 0x21 + 1 && (unsigned int)c < (unsigned int)('a' - 'z' - 1) && (unsigned int)(c + 'z' - '9') < (unsigned int)('0' - '9' - 1); }
static __inline int inline_isblank(int c)        { return c == ' ' || c == '\t'; }
static __inline int inline_isalnum(int c)        { unsigned int x = c; c |= 'a' - 'A'; x -= '0'; c -= 'a'; return (unsigned int)c < 'z' - 'a' + 1 || x < '9' - '0' + 1; }
static __inline int inline_isprint(int c)        { return (unsigned int)(c - 0x20) < 0x7E - 0x20 + 1; }
static __inline int inline_isgraph(int c)        { return (unsigned int)(c - 0x21) < 0x7E - 0x21 + 1; }
static __inline int inline_iscntrl(int c)        { return (unsigned int)c < 0x1F + 1 || c == 0x7F; }
static __inline int inline_isascii(int c)        { return (unsigned int)c < 0x7F + 1; }
static __inline int inline_tolower(int c)        { return (unsigned int)(c - 'A') >= 'Z' - 'A' + 1 ? c : c + 'a' - 'A'; }
static __inline int inline_toupper(int c)        { return (unsigned int)(c - 'a') >= 'z' - 'a' + 1 ? c : c + 'A' - 'a'; }
static __inline int inline_toascii(int c)        { return c & 0x7F; }
static __inline int inline_iswalpha(wint_t c)    { return (wint_t)((c | ('a' - 'A')) - 'a') < 'z' - 'a' + 1; }
static __inline int inline_iswupper(wint_t c)    { return (wint_t)(c - 'A') < 'Z' - 'A' + 1; }
static __inline int inline_iswlower(wint_t c)    { return (wint_t)(c - 'a') < 'z' - 'a' + 1; }
static __inline int inline_iswdigit(wint_t c)    { return (wint_t)(c - '0') < '9' - '0' + 1; }
static __inline int inline_iswxdigit(wint_t c)   { wint_t x = c; c |= 'a' - 'A'; x -= '0'; c -= 'a'; return x < '9' - '0' + 1 || c < 'f' - 'a' + 1; }
static __inline int inline_iswspace(wint_t c)    { return c == ' ' || (wint_t)(c - '\t') < '\r' - '\t' + 1; }
static __inline int inline_iswpunct(wint_t c)    { wint_t x = c; c |= 'a' - 'A'; x -= 0x21; c -= 'z' + 1; return x < 0x7E - 0x21 + 1 && c < (wint_t)('a' - 'z' - 1) && (wint_t)(c + 'z' - '9') < (wint_t)('0' - '9' - 1); }
static __inline int inline_iswblank(wint_t c)    { return c == ' ' || c == '\t'; }
static __inline int inline_iswalnum(wint_t c)    { wint_t x = c; c |= 'a' - 'A'; x -= '0'; c -= 'a'; return c < 'z' - 'a' + 1 || x < '9' - '0' + 1; }
static __inline int inline_iswprint(wint_t c)    { return (wint_t)(c - 0x20) < 0x7E - 0x20 + 1; }
static __inline int inline_iswgraph(wint_t c)    { return (wint_t)(c - 0x21) < 0x7E - 0x21 + 1; }
static __inline int inline_iswcntrl(wint_t c)    { return c < 0x1F + 1 || c == 0x7F; }
static __inline int inline_iswascii(wint_t c)    { return c < 0x7F + 1; }
static __inline wint_t inline_towlower(wint_t c) { return (wint_t)(c - 'A') >= 'Z' - 'A' + 1 ? c : c + 'a' - 'A'; }
static __inline wint_t inline_towupper(wint_t c) { return (wint_t)(c - 'a') >= 'z' - 'a' + 1 ? c : c + 'A' - 'a'; }
static __inline wint_t inline_towascii(wint_t c) { return c & 0x7F; }
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
	UINT   codepage;
	CPINFO cpinfo;

	if (GetLocaleInfoA(locale, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER, (LPSTR)&codepage, sizeof(codepage) / sizeof(char)) &&
		GetCPInfo(codepage, &cpinfo))
	{
		regex_lcid = locale;
		regex_codepage = codepage;
		regex_mb_cur_max = cpinfo.MaxCharSize;
		return locale;
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

#ifdef _MSC_VER
static __inline int wcscoll(const wchar_t *string1, const wchar_t *string2)
{
	int ret;
	return (ret = CompareStringW(LOCALE_ID, SORT_STRINGSORT, string1, -1, string2, -1)) ?
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

static __inline bool is_utf8()
{
	UINT codepage;

	return
		GetLocaleInfoA(LOCALE_ID, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER, (LPSTR)&codepage, sizeof(codepage) / sizeof(char)) &&
		codepage == CP_UTF8;
}

#define nl_langinfo(dummy_item) (is_utf8() ? "UTF-8" : "")

#pragma warning(disable: 4018 4244)
#include "regex.c"

