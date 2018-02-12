#ifndef _DIGITSTBL_H_
#define _DIGITSTBL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

const char    digitsHexLarge[36];
const char    digitsHexSmall[36];
const char    digitsDec100A[100][2];
const wchar_t digitsDec100W[100][2];

#ifdef __cplusplus
}
#endif

#endif	// _DIGITSTBL_H_
