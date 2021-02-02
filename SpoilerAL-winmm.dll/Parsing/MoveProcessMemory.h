#ifndef _MOVEPROCESSMEMORY_H_
#define _MOVEPROCESSMEMORY_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>
#include "ntstatus.h"

#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS __stdcall InternalMoveProcessMemory(
	IN          BOOL    bHandleOverlapRegions,
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize);

#define CopyProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize) \
	InternalMoveProcessMemory(FALSE, hDestProcess, lpDest, hSrcProcess, lpSrc, nSize)

#define MoveProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize) \
	InternalMoveProcessMemory(TRUE, hDestProcess, lpDest, hSrcProcess, lpSrc, nSize)

#ifdef __cplusplus
}
#endif

#endif	// _MOVEPROCESSMEMORY_H_
