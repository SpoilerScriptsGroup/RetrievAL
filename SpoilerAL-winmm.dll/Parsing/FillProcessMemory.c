#ifndef _DEBUG
#include "noinline_wchar.h"
#else
#include <wchar.h>
#endif
#include <windows.h>
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "page.h"

#pragma intrinsic(__debugbreak)

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma function(memset)
#endif

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
			__declspec(align(16)) BYTE lpBuffer[PAGE_SIZE];

			if (nCount > PAGE_SIZE)
			{
				size_t nAlign;
				size_t nSize;

				memset(lpBuffer, bFill, min(nCount, PAGE_SIZE));
				nSize = nCount;
				if (nAlign = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize >> PAGE_SHIFT)
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, PAGE_SIZE, NULL))
							return FALSE;
						(LPBYTE)lpDest += PAGE_SIZE;
					} while (--nCount);
				}
				if (nSize &= (PAGE_SIZE - 1))
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

		if (nCount >= ((size_t)1 << (sizeof(size_t) * 8 - 1)))
			return FALSE;
		nSize = nCount * 2;
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			__declspec(align(16)) BYTE lpBuffer[PAGE_SIZE];

			if (nSize > PAGE_SIZE)
			{
				LPBYTE lpSrc;
				size_t nAlign;

				lpSrc = lpBuffer;
				nCount = min(nCount, PAGE_SIZE / 2);
				if ((size_t)lpDest & 1)
				{
					lpSrc++;
					wFill = _byteswap_ushort(wFill);
					nCount += nCount < PAGE_SIZE / 2;
				}
				wmemset((wchar_t *)lpBuffer, wFill, nCount);
				if (nAlign = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpSrc, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize >> PAGE_SHIFT)
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, PAGE_SIZE, NULL))
							return FALSE;
						(LPBYTE)lpDest += PAGE_SIZE;
					} while (--nCount);
				}
				if (nSize &= (PAGE_SIZE - 1))
					return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
			}
			else
			{
				wmemset((wchar_t *)lpBuffer, wFill, nCount);
#pragma warning(push)
#pragma warning(disable:6001)
				return WriteProcessMemory(hProcess, lpDest, lpBuffer, nSize, NULL);
#pragma warning(pop)
			}
		}
		else
		{
			if (IsBadWritePtr(lpDest, nSize))
				return FALSE;
			wmemset(lpDest, wFill, nCount);
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

		if (nCount >= ((size_t)1 << (sizeof(size_t) * 8 - 2)))
			return FALSE;
		nSize = nCount * 4;
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			__declspec(align(16)) BYTE lpBuffer[PAGE_SIZE];

			if (nSize > PAGE_SIZE)
			{
				LPBYTE lpSrc;
				size_t nShift, nAlign;

				lpSrc = lpBuffer;
				nCount = min(nCount, PAGE_SIZE / 4);
				if (nShift = (size_t)lpDest & 3)
				{
					lpSrc += nShift;
					dwFill = _rotl(dwFill, nShift * 8);
					nCount += nCount < PAGE_SIZE / 4;
				}
				__stosd((unsigned long *)lpBuffer, dwFill, nCount);
				if (nAlign = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpSrc, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize >> PAGE_SHIFT)
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, PAGE_SIZE, NULL))
							return FALSE;
						(LPBYTE)lpDest += PAGE_SIZE;
					} while (--nCount);
				}
				if (nSize &= (PAGE_SIZE - 1))
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

		if (nCount >= ((size_t)1 << (sizeof(size_t) * 8 - 3)))
			return FALSE;
		nSize = nCount * 8;
		if (hProcess && GetProcessId(hProcess) != GetCurrentProcessId())
		{
			__declspec(align(16)) BYTE lpBuffer[PAGE_SIZE];

			if (nSize > PAGE_SIZE)
			{
				LPBYTE lpSrc;
				size_t nShift, nAlign;

				lpSrc = lpBuffer;
				nCount = min(nCount, PAGE_SIZE / 8);
				if (nShift = (size_t)lpDest & 7)
				{
					lpSrc += nShift;
					qwFill = _rotl64(qwFill, nShift * 8);
					nCount += nCount < PAGE_SIZE / 8;
				}
				__stosq((unsigned __int64 *)lpBuffer, qwFill, nCount);
				if (nAlign = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hProcess, lpDest, lpSrc, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						return TRUE;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize >> PAGE_SHIFT)
				{
					do
					{
						if (!WriteProcessMemory(hProcess, lpDest, lpBuffer, PAGE_SIZE, NULL))
							return FALSE;
						(LPBYTE)lpDest += PAGE_SIZE;
					} while (--nCount);
				}
				if (nSize &= (PAGE_SIZE - 1))
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
