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

#ifdef __cplusplus
}
#endif

#define WCTOI(p, max, base)       ((wchar_t)*(p) <= (max) && (wchar_t)(*(p) = atoitbl[(wchar_t)*(p)]) < (wchar_t)(base))
#define ACTOI(p, max, base)       ((unsigned char)(*(p) = atoitbl[(unsigned char)*(p)]) < (unsigned char)(base))
#define CHECK_WCTOI(c, max, base) ((wchar_t)(c) <= (max) && atoitbl[(wchar_t)(c)] < (unsigned char)(base))
#define CHECK_ACTOI(c, max, base) (atoitbl[(unsigned char)(c)] < (unsigned char)(base))

#ifdef _UNICODE
#define CTOI       WCTOI
#define CHECK_CTOI CHECK_WCTOI
#else
#define CTOI       ACTOI
#define CHECK_CTOI CHECK_ACTOI
#endif

#endif	// _ATOITBL_H_
