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

#define WCTOI(p, max, base)       ((wchar_t)*(p) <= (max) && (wchar_t)(*(p) = wtoitbl[(wchar_t)*(p)]) < (wchar_t)(base))
#define ACTOI(p, max, base)       ((unsigned char)(*(p) = atoitbl[(unsigned char)*(p)]) < (unsigned char)(base))
#define CHECK_WCTOI(c, max, base) ((wchar_t)(c) <= (max) && wtoitbl[(wchar_t)(c)] < (wchar_t)(base))
#define CHECK_ACTOI(c, max, base) (atoitbl[(unsigned char)(c)] < (unsigned char)(base))

#ifdef _UNICODE
#define ttoitbl    wtoitbl
#define CTOI       WCTOI
#define CHECK_CTOI CHECK_WCTOI
#else
#define ttoitbl    atoitbl
#define CTOI       ACTOI
#define CHECK_CTOI CHECK_ACTOI
#endif

#endif	// _ATOITBL_H_
