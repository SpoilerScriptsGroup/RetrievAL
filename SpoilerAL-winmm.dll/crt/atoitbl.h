#ifndef _ATOITBL_H_
#define _ATOITBL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned char atoitbl[256];
extern const wchar_t wtoitbl[128];

#ifdef __cplusplus
}
#endif

#define _WCTOI(i, c, max, base)    ((wchar_t)(c) <= (max) && (wchar_t)(*(i) = wtoitbl[(wchar_t)(c)]) < (wchar_t)(base))
#define _ACTOI(i, c, max, base)    ((unsigned char)(*(i) = atoitbl[(unsigned char)(c)]) < (unsigned char)(base))
#define WCTOI(i, c, base)          _WCTOI(i, c, L'z', base)
#define ACTOI(i, c, base)          _ACTOI(i, c, 'z', base)
#define WCTOX(i, c)                _WCTOI(i, c, L'f', 16)
#define ACTOX(i, c)                _ACTOI(i, c, 'f', 16)
#define _CHECK_WCTOI(c, max, base) ((wchar_t)(c) <= (max) && wtoitbl[(wchar_t)(c)] < (wchar_t)(base))
#define _CHECK_ACTOI(c, max, base) (atoitbl[(unsigned char)(c)] < (unsigned char)(base))
#define CHECK_WCTOI(c, base)       _CHECK_WCTOI(c, L'z', base)
#define CHECK_ACTOI(c, base)       _CHECK_ACTOI(c, 'z', base)
#define CHECK_WCTOX(c)             _CHECK_WCTOI(c, L'f', 16)
#define CHECK_ACTOX(c)             _CHECK_ACTOI(c, 'f', 16)

#ifdef _UNICODE
#define ttoitbl     wtoitbl
#define _TCTOI      _WCTOI
#define TCTOI       WCTOI
#define TCTOX       WCTOX
#define CHECK_TCTOI CHECK_WCTOI
#define CHECK_TCTOX CHECK_WCTOX
#else
#define ttoitbl     atoitbl
#define _TCTOI      _ACTOI
#define TCTOI       ACTOI
#define TCTOX       ACTOX
#define CHECK_TCTOI CHECK_ACTOI
#define CHECK_TCTOX CHECK_ACTOX
#endif

#endif	// _ATOITBL_H_
