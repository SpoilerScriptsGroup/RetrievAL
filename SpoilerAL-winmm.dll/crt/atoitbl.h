#ifndef _ATOITBL_H_
#define _ATOITBL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

EXTERN_C const unsigned char atoitbl[256];

#ifdef _UNICODE
__forceinline unsigned char ATOITBL(wchar_t c) { return c <= 'z' ? atoitbl[c] : 0xFF; }
#else
#define ATOITBL(c) atoitbl[(unsigned char)(c)]
#endif

#endif	// _ATOITBL_H_
