#ifndef _GETPAGESIZE_H_
#define _GETPAGESIZE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8065
#endif

EXTERN_C DWORD __stdcall GetPageSize();

#ifdef __cplusplus
}
#endif

#endif	// _GETPAGESIZE_H_
