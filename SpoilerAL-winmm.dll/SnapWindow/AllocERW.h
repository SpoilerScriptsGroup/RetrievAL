#ifndef _ALLOCERW_H_
#define _ALLOCERW_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#define AllocERW(nSize) VirtualAlloc(NULL, nSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE)
#define FreeERW(lpAddress) VirtualFree(lpAddress, 0, MEM_RELEASE)
LPVOID __stdcall ReAllocERW(LPVOID lpAddress, size_t nSize);

#ifdef __cplusplus
}
#endif

#endif	// _ALLOCERW_H_
