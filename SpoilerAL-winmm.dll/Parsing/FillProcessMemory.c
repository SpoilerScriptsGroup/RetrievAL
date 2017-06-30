#include <windows.h>
#include <intrin.h>
#include "IsBadXxxPtr.h"

#pragma intrinsic(memset)
#pragma intrinsic(__stosd)

#define __stosw(Dest, Data, Count)                                   \
do {                                                                 \
    __restrict unsigned short *__Dest  = Dest;                       \
    __restrict unsigned short  __Data  = Data;                       \
    __restrict size_t          __Count = Count;                      \
                                                                     \
    if (__Count & 1)                                                 \
        __Dest[__Count - 1] = __Data;                                \
    if (__Count >>= 1)                                               \
        __stosd((LPDWORD)__Dest, MAKELONG(__Data, __Data), __Count); \
} while (0)

BOOL __stdcall FillProcessMemory(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          BYTE   nFill)
{
	if (nCount)
	{
		if (hProcess)
		{
			BYTE lpBuffer[4096];

			if (nCount > sizeof(lpBuffer))
			{
				size_t nAlign;
				size_t nSize;

				memset(lpBuffer, nFill, sizeof(lpBuffer));
				nSize = nCount;
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
				{
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
						return FALSE;
				}
			}
			else
			{
				memset(lpBuffer, nFill, nCount);
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nCount, NULL));
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nCount))
				return FALSE;
			memset(lpDest, nFill, nCount);
		}
	}
	return TRUE;
}

BOOL __stdcall FillWordProcessMemory(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          WORD   nFill)
{
	if (nCount)
	{
		size_t nSize;

		nSize = nCount * 2;
		if (hProcess)
		{
			BYTE lpBuffer[4096];

			if (nSize > sizeof(lpBuffer))
			{
				size_t nAlign;

				__stosd(lpBuffer, MAKELONG(nFill, nFill), sizeof(lpBuffer) / 4);
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
				{
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
						return FALSE;
				}
			}
			else
			{
				__stosw(lpDest, nFill, nCount);
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL));
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nSize))
				return FALSE;
			__stosw(lpDest, nFill, nCount);
		}
	}
	return TRUE;
}

BOOL __stdcall FillDWordProcessMemory(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          DWORD  nFill)
{
	if (nCount)
	{
		size_t nSize;

		nSize = nCount * 4;
		if (hProcess)
		{
			BYTE lpBuffer[4096];

			if (nSize > sizeof(lpBuffer))
			{
				size_t nAlign;

				__stosd((wchar_t *)lpBuffer, nFill, sizeof(lpBuffer) / 4);
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
				{
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
						return FALSE;
				}
			}
			else
			{
				__stosd(lpBuffer, nFill, nCount);
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL));
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nSize))
				return FALSE;
			__stosd(lpDest, nFill, nCount);
		}
	}
	return TRUE;
}
