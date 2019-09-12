#ifndef _DIGITSTBL_H_
#define _DIGITSTBL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const char    digitsLargeA[36];
extern const wchar_t digitsLargeW[36];
extern const char    digitsSmallA[36];
extern const wchar_t digitsSmallW[36];
extern const char    digits100A[100][2];
extern const wchar_t digits100W[100][2];

#ifdef __cplusplus
}
#endif

#ifdef _UNICODE
#define digitsLarge digitsLargeW
#define digitsSmall digitsSmallW
#define digits100   digits100W
#else
#define digitsLarge digitsLargeA
#define digitsSmall digitsSmallA
#define digits100   digits100A
#endif

#endif	// _DIGITSTBL_H_
