#include "CompareProcessMemory.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "PageSize.h"
#include <stdint.h>
#include <tchar.h>

#ifdef _UNICODE
#define CompareProcessMemoryT CompareProcessMemoryW
#define _tmemcmp wmemcmp
#else
#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma function(memcmp)
#endif
#define CompareProcessMemoryT CompareProcessMemoryA
#define _tmemcmp memcmp
#endif

#ifdef _DEBUG
#include <assert.h>
# define ASSERT assert
#else
# define ASSERT __assume
#endif

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

NTSTATUS __stdcall CompareProcessMemoryT(
	OUT         int     *lpiResult,
	IN OPTIONAL HANDLE  hProcess1,
	IN          LPCVOID lpAddress1,
	IN OPTIONAL HANDLE  hProcess2,
	IN          LPCVOID lpAddress2,
	IN          size_t  nCount)
{
	__declspec(align(16)) BYTE lpBuffer1[PAGE_SIZE + sizeof(TCHAR) - 1];
	__declspec(align(16)) BYTE lpBuffer2[PAGE_SIZE + sizeof(TCHAR) - 1];
	BOOLEAN                    bIsSameProcess;
	size_t                     nSize, nRead;
#ifdef _UNICODE
	size_t                     nRemainder;
#else
	#define nRemainder 0
#endif

	*lpiResult = 0;
	if (!nCount)
		goto SUCCESS;
	if (hProcess1 != hProcess2)
	{
		DWORD dwCurPID, dwPID1, dwPID2;

		dwPID2 = dwPID1 = dwCurPID = GetCurrentProcessId();
		if (hProcess1)
		{
			dwPID1 = GetProcessId(hProcess1);
			if (!dwPID1)
				goto ACCESS_DENIED;
			if (dwPID1 == dwCurPID)
				hProcess1 = NULL;
		}
		if (hProcess2)
		{
			dwPID2 = GetProcessId(hProcess2);
			if (!dwPID2)
				goto ACCESS_DENIED;
			if (dwPID2 == dwCurPID)
				hProcess2 = NULL;
		}
		bIsSameProcess = dwPID1 == dwPID2;
	}
	else
	{
		if (hProcess1)
		{
			DWORD dwProcessId;

			dwProcessId = GetProcessId(hProcess1);
			if (!dwProcessId)
				goto ACCESS_DENIED;
			if (dwProcessId == GetCurrentProcessId())
				hProcess2 = hProcess1 = NULL;
		}
		bIsSameProcess = TRUE;
	}
	if (bIsSameProcess && lpAddress1 == lpAddress2)
		goto SUCCESS;
#ifdef _UNICODE
	nSize = min(nCount, UINTPTR_MAX / sizeof(TCHAR)) * sizeof(TCHAR);
#else
	nSize = nCount;
#endif
	if (hProcess1)
	{
		if (hProcess2)
		{
			size_t nRead1, nRead2, nCompare, nOffset;
#ifdef _UNICODE
			size_t nRemainder1, nRemainder2;
#else
			#define nRemainder1 0
			#define nRemainder2 0
#endif

			nRead1 = min(PAGE_SIZE - ((size_t)lpAddress1 & (PAGE_SIZE - 1)), nSize);
			ASSERT(nRead1 <= PAGE_SIZE);
			if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, nRead1, NULL))
				goto READ1_FAILED;
			nRead2 = min(PAGE_SIZE - ((size_t)lpAddress2 & (PAGE_SIZE - 1)), nSize);
			ASSERT(nRead2 <= PAGE_SIZE);
			if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, nRead2, NULL))
				goto READ2_FAILED;
			nOffset = nCompare = min(nRead1, nRead2);
#ifdef _UNICODE
			if (nOffset &= -2)
#endif
				if (*lpiResult = _tmemcmp((TCHAR *)lpBuffer1, (TCHAR *)lpBuffer2, nCompare / sizeof(TCHAR)))
					goto SUCCESS;
			if (!(nSize -= nCompare))
				goto SUCCESS;
			(LPBYTE)lpAddress1 += nRead1;
			(LPBYTE)lpAddress2 += nRead2;
#ifdef _UNICODE
			if (nRemainder1 = (size_t)nRead1 & 1)
				lpBuffer1[0] = lpBuffer1[--nRead1];
			if (nRemainder2 = (size_t)nRead2 & 1)
				lpBuffer2[0] = lpBuffer2[--nRead2];
#endif
			if (_sub_uintptr(nRead2, nRead1, &nCompare))
			{
				nCompare = -(ptrdiff_t)nCompare;
				goto READ_BUFFER2;
			}
			for (; ; )
			{
				if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1 + nRemainder1, min(PAGE_SIZE, nSize), NULL))
					goto READ1_FAILED;
				if (nCompare)
				{
					nCompare = min(nCompare, nSize);
					ASSERT(nCompare <= PAGE_SIZE);
					if (*lpiResult = _tmemcmp((TCHAR *)lpBuffer1, (TCHAR *)(lpBuffer2 + nOffset), (nCompare + sizeof(TCHAR) - 1) / sizeof(TCHAR)))
						break;
					if (!(nSize -= nCompare))
						break;
				}
				if (nRemainder1)
					lpBuffer1[0] = lpBuffer1[sizeof(lpBuffer1) - 1];
				(LPBYTE)lpAddress1 += PAGE_SIZE;
				nCompare = PAGE_SIZE - (nOffset = nCompare);
			READ_BUFFER2:
				if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2 + nRemainder2, min(PAGE_SIZE, nSize), NULL))
					goto READ2_FAILED;
#ifdef _UNICODE
				if (nCompare)
#endif
				{
					nCompare = min(nCompare, nSize);
					ASSERT(nCompare <= PAGE_SIZE);
					if (*lpiResult = _tmemcmp((TCHAR *)(lpBuffer1 + nOffset), (TCHAR *)lpBuffer2, (nCompare + sizeof(TCHAR) - 1) / sizeof(TCHAR)))
						break;
					if (!(nSize -= nCompare))
						break;
				}
				if (nRemainder2)
					lpBuffer2[0] = lpBuffer2[sizeof(lpBuffer2) - 1];
				(LPBYTE)lpAddress2 += PAGE_SIZE;
				nCompare = PAGE_SIZE - (nOffset = nCompare);
			}

#ifndef _UNICODE
			#undef nRemainder1
			#undef nRemainder2
#endif
		}
		else
		{
			if (IsBadReadPtr(lpAddress2, nSize))
				goto READ2_FAILED;
#ifdef _UNICODE
			nRemainder = (size_t)lpAddress1 & 1;
#endif
			if (nRead = -(ptrdiff_t)lpAddress1 & (PAGE_SIZE - 1))
			{
				nRead = min(nRead, nSize);
				ASSERT(nRead < PAGE_SIZE);
				if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, nRead, NULL))
					goto READ1_FAILED;
				nCount = nRead;
#ifdef _UNICODE
				if (nCount /= sizeof(TCHAR))
#endif
					if (*lpiResult = _tmemcmp((TCHAR *)lpBuffer1, lpAddress2, nCount))
						goto SUCCESS;
				if (!(nSize -= nRead))
					goto SUCCESS;
				if (nRemainder)
					lpBuffer1[0] = lpBuffer1[nRead - 1];
				(LPBYTE)lpAddress1 += nRead;
				(LPBYTE)lpAddress2 += nRead;
			}
			if (nCount = nSize >> BSF(PAGE_SIZE))
			{
				do
				{
					if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1 + nRemainder, PAGE_SIZE, NULL))
						goto READ1_FAILED;
					if (*lpiResult = _tmemcmp((TCHAR *)lpBuffer1, lpAddress2, PAGE_SIZE / sizeof(TCHAR)))
						goto SUCCESS;
					if (nRemainder)
						lpBuffer1[0] = lpBuffer1[sizeof(lpBuffer1) - 1];
					(LPBYTE)lpAddress1 += PAGE_SIZE;
					(LPBYTE)lpAddress2 += PAGE_SIZE;
				} while (--nCount);
			}
			if (nSize &= PAGE_SIZE - 1)
			{
				if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1 + nRemainder, nSize, NULL))
					goto READ1_FAILED;
				*lpiResult = _tmemcmp((TCHAR *)lpBuffer1, lpAddress2, (nSize + sizeof(TCHAR) - 1) / sizeof(TCHAR));
			}
		}
	}
	else
	{
		if (hProcess2)
		{
			if (IsBadReadPtr(lpAddress1, nSize))
				goto READ1_FAILED;
#ifdef _UNICODE
			nRemainder = (size_t)lpAddress2 & 1;
#endif
			if (nRead = -(ptrdiff_t)lpAddress2 & (PAGE_SIZE - 1))
			{
				nRead = min(nRead, nSize);
				ASSERT(nRead < PAGE_SIZE);
				if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, nRead, NULL))
					goto READ2_FAILED;
				nCount = nRead;
#ifdef _UNICODE
				if (nCount /= sizeof(TCHAR))
#endif
					if (*lpiResult = _tmemcmp(lpAddress1, (TCHAR *)lpBuffer2, nCount))
						goto SUCCESS;
				if (!(nSize -= nRead))
					goto SUCCESS;
				if (nRemainder)
					lpBuffer2[0] = lpBuffer2[nRead - 1];
				(LPBYTE)lpAddress1 += nRead;
				(LPBYTE)lpAddress2 += nRead;
			}
			if (nCount = nSize >> BSF(PAGE_SIZE))
			{
				do
				{
					if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2 + nRemainder, PAGE_SIZE, NULL))
						goto READ2_FAILED;
					if (*lpiResult = _tmemcmp(lpAddress1, (TCHAR *)lpBuffer2, PAGE_SIZE / sizeof(TCHAR)))
						goto SUCCESS;
					if (nRemainder)
						lpBuffer2[0] = lpBuffer2[sizeof(lpBuffer2) - 1];
					(LPBYTE)lpAddress1 += PAGE_SIZE;
					(LPBYTE)lpAddress2 += PAGE_SIZE;
				} while (--nCount);
			}
			if (nSize &= PAGE_SIZE - 1)
			{
				if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2 + nRemainder, nSize, NULL))
					goto READ2_FAILED;
				*lpiResult = _tmemcmp(lpAddress1, (TCHAR *)lpBuffer2, (nSize + sizeof(TCHAR) - 1) / sizeof(TCHAR));
			}
		}
		else
		{
			if (IsBadReadPtr(lpAddress1, nSize))
				goto READ1_FAILED;
			if (IsBadReadPtr(lpAddress2, nSize))
				goto READ2_FAILED;
			*lpiResult = _tmemcmp(lpAddress1, lpAddress2, nCount);
		}
	}

SUCCESS:
	return STATUS_SUCCESS;

ACCESS_DENIED:
	return STATUS_PROCESS_ACCESS_DENIED;

READ1_FAILED:
	return STATUS_MEMORY_READ1_FAILED;

READ2_FAILED:
	return STATUS_MEMORY_READ2_FAILED;

#ifndef _UNICODE
	#undef nRemainder
#endif
}
