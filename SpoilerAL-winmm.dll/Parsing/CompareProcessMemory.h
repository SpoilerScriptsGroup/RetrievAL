#ifndef _COMPAREPROCESSMEMORY_H_
#define _COMPAREPROCESSMEMORY_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>
#include "ntstatus.h"

#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS __stdcall CompareProcessMemoryA(
	OUT         int     *lpiResult,
	IN OPTIONAL HANDLE  hProcess1,
	IN          LPCVOID lpAddress1,
	IN OPTIONAL HANDLE  hProcess2,
	IN          LPCVOID lpAddress2,
	IN          size_t  nCount,
	IN          BOOL    bInsensitiveCase);

NTSTATUS __stdcall CompareProcessMemoryW(
	OUT         int     *lpiResult,
	IN OPTIONAL HANDLE  hProcess1,
	IN          LPCVOID lpAddress1,
	IN OPTIONAL HANDLE  hProcess2,
	IN          LPCVOID lpAddress2,
	IN          size_t  nCount,
	IN          BOOL    bInsensitiveCase);

#ifdef __cplusplus
}
#endif

#endif	// _COMPAREPROCESSMEMORY_H_
