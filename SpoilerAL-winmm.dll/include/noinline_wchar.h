#ifndef _NOINLINE_WCHAR_H_
#define _NOINLINE_WCHAR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define wmemchr inline_wmemchr
#define wmemcmp inline_wmemcmp
#define wmemset inline_wmemset
#include <wchar.h>
#undef wmemchr
#undef wmemcmp
#undef wmemset

#ifdef __cplusplus
extern "C" {
#endif

extern int wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
extern wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl wmemset(wchar_t *dest, wchar_t c, size_t count);

#ifdef __cplusplus
}
#endif

#endif	// _NOINLINE_WCHAR_H_
