#ifdef __BORLANDC__
#pragma warn -8060
#endif
#include <windows.h>
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "PageSize.h"

#ifdef __BORLANDC__
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(memset)
#endif

#undef __stosw
#define __stosw(Dest, Data, Count)                               \
do                                                               \
{                                                                \
    unsigned short *__restrict _Dest  = Dest;                    \
    unsigned short             _Data  = Data;                    \
    size_t                     _Count = Count;                   \
                                                                 \
    if (_Count & 1)                                              \
        _Dest[_Count - 1] = _Data;                               \
    if (_Count >>= 1)                                            \
        __stosd((LPDWORD)_Dest, MAKELONG(_Data, _Data), _Count); \
} while (0)

BOOL __stdcall FillProcessMemory(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          BYTE   bFill)
{
	if (nCount)
	{
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			BYTE lpBuffer[PAGE_SIZE];

			if (nCount > sizeof(lpBuffer))
			{
				size_t nAlign;
				size_t nSize;

				memset(lpBuffer, bFill, sizeof(lpBuffer));
				nSize = nCount;
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
					return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
			else
			{
				memset(lpBuffer, bFill, nCount);
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nCount, NULL);
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nCount))
				return FALSE;
			memset(lpDest, bFill, nCount);
		}
	}
	return TRUE;
}

BOOL __stdcall FillProcessMemory16(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          WORD   wFill)
{
	if (nCount)
	{
		size_t nSize;

		nSize = nCount * 2;
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			BYTE lpBuffer[PAGE_SIZE];

			if (nSize > sizeof(lpBuffer))
			{
				size_t nAlign;

				__stosd((unsigned long *)lpBuffer, MAKELONG(wFill, wFill), sizeof(lpBuffer) / 4);
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
					return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
			else
			{
				__stosw(lpBuffer, wFill, nCount);
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nSize))
				return FALSE;
			__stosw(lpDest, wFill, nCount);
		}
	}
	return TRUE;
}

BOOL __stdcall FillProcessMemory32(
	IN OPTIONAL HANDLE hProcess,
	IN          PVOID  lpDest,
	IN          size_t nCount,
	IN          DWORD  dwFill)
{
	if (nCount)
	{
		size_t nSize;

		nSize = nCount * 4;
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			BYTE lpBuffer[PAGE_SIZE];

			if (nSize > sizeof(lpBuffer))
			{
				size_t nAlign;

				__stosd((unsigned long *)lpBuffer, dwFill, sizeof(lpBuffer) / 4);
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
					return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
			else
			{
				__stosd((unsigned long *)lpBuffer, dwFill, nCount);
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nSize))
				return FALSE;
			__stosd(lpDest, dwFill, nCount);
		}
	}
	return TRUE;
}

BOOL __stdcall FillProcessMemory64(
	IN OPTIONAL HANDLE           hProcess,
	IN          PVOID            lpDest,
	IN          size_t           nCount,
	IN          unsigned __int64 qwFill)
{
	if (nCount)
	{
		size_t nSize;

		nSize = nCount * 8;
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			BYTE lpBuffer[PAGE_SIZE];

			if (nSize > sizeof(lpBuffer))
			{
				size_t nAlign;

				__stosq((unsigned __int64 *)lpBuffer, qwFill, sizeof(lpBuffer) / 8);
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
					return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
			else
			{
				__stosq((unsigned __int64 *)lpBuffer, qwFill, nCount);
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nSize))
				return FALSE;
			__stosq(lpDest, qwFill, nCount);
		}
	}
	return TRUE;
}
