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

NTSTATUS __stdcall CopyProcessMemory(
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize);

NTSTATUS __stdcall MoveProcessMemory(
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize);

#ifdef __cplusplus
}
#endif

#endif	// _MOVEPROCESSMEMORY_H_
