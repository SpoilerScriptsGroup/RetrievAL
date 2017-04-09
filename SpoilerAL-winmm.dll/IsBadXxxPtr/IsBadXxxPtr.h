#ifndef _ISBADXXXPTR_H_
#define _ISBADXXXPTR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall IsBadReadPtrCompatible(IN LPCVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadWritePtrCompatible(IN LPVOID lp, IN UINT_PTR ucb);

#define IsBadReadPtr  IsBadReadPtrCompatible
#define IsBadWritePtr IsBadWritePtrCompatible

#ifdef __cplusplus
}
#endif

#endif	// _ISBADXXXPTR_H_
