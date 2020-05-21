#ifndef _DIGITSTBL_H_
#define _DIGITSTBL_H_

#if _MSC_VER > 1000
#pragma once
#endif

typedef unsigned short wchar_t;

#ifdef __cplusplus
extern "C" {
#endif

#define ptrdiff_digitsSmallA ((36 + 15) & -16)
#define ptrdiff_digitsSmallW ((36 * 2 + 15) & -16)

extern const char    digitsLargeA[ptrdiff_digitsSmallA + 36];
extern const wchar_t digitsLargeW[ptrdiff_digitsSmallW / sizeof(wchar_t) + 36];
#define digitsSmallA (digitsLargeA + ptrdiff_digitsSmallA)
#define digitsSmallW (digitsLargeW + ptrdiff_digitsSmallW / sizeof(wchar_t))
extern const char    digits100A[100][2];
extern const wchar_t digits100W[100][2];

#ifdef __cplusplus
}
#endif

#ifndef _UNICODE
#define ptrdiff_digitsSmall ptrdiff_digitsSmallA
#define digitsLarge         digitsLargeA
#define digitsSmall         digitsSmallA
#define digits100           digits100A
#else
#define ptrdiff_digitsSmall ptrdiff_digitsSmallW
#define digitsLarge         digitsLargeW
#define digitsSmall         digitsSmallW
#define digits100           digits100W
#endif

#endif	// _DIGITSTBL_H_
