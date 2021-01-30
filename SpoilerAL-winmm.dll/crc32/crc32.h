#ifndef _CRC32_H_
#define _CRC32_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int __fastcall CRC32Combine(unsigned long int uPreviousCrc32, const void *lpBuffer, unsigned long int uSize);
#define CRC32(lpBuffer, uSize) CRC32Combine(0, lpBuffer, uSize)

#ifdef _WIN32

#include <windows.h>
#include <stdio.h>
#include <io.h>

DWORD __fastcall CRC32FromFileHandle(IN HANDLE hFile);
__inline DWORD CRC32FromFileStruct(IN FILE *file)
{
	return CRC32FromFileHandle((HANDLE)_get_osfhandle(_fileno(file)));
}
BOOL __fastcall CRC32FromFileA(IN LPCSTR lpFileName, OUT LPDWORD lpdwCrc);
BOOL __fastcall CRC32FromFileW(IN LPCWSTR lpFileName, OUT LPDWORD lpdwCrc);
#ifdef _UNICODE
#define CRC32FromFile CRC32FromFileW
#else
#define CRC32FromFile CRC32FromFileA
#endif

#endif /* _WIN32 */

#ifdef __cplusplus
}
#endif

#endif /* _CRC32_H_ */
