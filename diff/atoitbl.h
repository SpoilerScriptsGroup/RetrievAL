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

#ifdef _UNICODE
#define CTOI(p, max, base) ((wchar_t)*(p) <= (max) && (wchar_t)(*(p) = atoitbl[(wchar_t)*(p)]) < (wchar_t)(base))
#define CHECK_CTOI(c, max, base) ((wchar_t)(c) <= (max) && atoitbl[(wchar_t)(c)] < (unsigned char)(base))
#else
#define CTOI(p, max, base) ((unsigned char)(*(p) = atoitbl[(unsigned char)*(p)]) < (unsigned char)(base))
#define CHECK_CTOI(c, max, base) (atoitbl[(unsigned char)(c)] < (unsigned char)(base))
#endif

#endif	// _ATOITBL_H_
