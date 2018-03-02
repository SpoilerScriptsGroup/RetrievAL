#ifndef _DIGITSTBL_H_
#define _DIGITSTBL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const char    digitsLarge[36];
extern const char    digitsSmall[36];
extern const char    digits100A[100][2];
extern const wchar_t digits100W[100][2];

#ifdef __cplusplus
}
#endif

#endif	// _DIGITSTBL_H_
