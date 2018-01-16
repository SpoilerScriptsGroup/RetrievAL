#ifndef _CRC32_H_
#define _CRC32_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int CRC32Combine(const void *lpBuffer, unsigned long int nSize, unsigned long int crc);
#define CRC32(lpBuffer, nSize) CRC32Combine(lpBuffer, nSize, 0)

#ifdef _WIN32

#include <windows.h>
#include <stdio.h>
#include <io.h>

DWORD CRC32FromFileHandle(IN HANDLE hFile);
__inline DWORD CRC32FromFileStruct(IN FILE *file)
{
	return CRC32FromFileHandle((HANDLE)_get_osfhandle(_fileno(file)));
}
BOOL CRC32FromFileA(IN LPCSTR lpFileName, OUT LPDWORD crc);
BOOL CRC32FromFileW(IN LPCWSTR lpFileName, OUT LPDWORD crc);
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
