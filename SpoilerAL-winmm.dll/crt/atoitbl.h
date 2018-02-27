#ifndef _ATOITBL_H_
#define _ATOITBL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

EXTERN_C const unsigned char atoitbl[256];

#ifdef _UNICODE
#define ATOITBL(c) atoitbl[(wchar_t)(c)]
#else
#define ATOITBL(c) atoitbl[(unsigned char)(c)]
#endif

#endif	// _ATOITBL_H_
