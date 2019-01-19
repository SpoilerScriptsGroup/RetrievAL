#include "MemoryCompare.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "PageSize.h"

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

NTSTATUS __stdcall MemoryCompare(
	OUT         int     *lpiResult,
	IN OPTIONAL HANDLE  hProcess1,
	IN          LPCVOID lpAddress1,
	IN OPTIONAL HANDLE  hProcess2,
	IN          LPCVOID lpAddress2,
	IN          size_t  nSize)
{
	BYTE    lpBuffer1[PAGE_SIZE];
	BYTE    lpBuffer2[PAGE_SIZE];
	BOOLEAN bIsSameProcess;
	size_t  nAlign;
	size_t  nCount;

	*lpiResult = 0;
	if (!nSize)
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
	if (hProcess1)
	{
		if (hProcess2)
		{
			if (nAlign = -(ptrdiff_t)lpAddress1 & (PAGE_SIZE - 1))
			{
				if (nAlign > nSize)
					nAlign = nSize;
				if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, nAlign, NULL))
					goto READ1_FAILED;
				if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, nAlign, NULL))
					goto READ2_FAILED;
				if (*lpiResult = memcmp(lpBuffer1, lpBuffer2, nAlign))
					goto SUCCESS;
				if (!(nSize -= nAlign))
					goto SUCCESS;
				(LPBYTE)lpAddress1 += nAlign;
				(LPBYTE)lpAddress2 += nAlign;
			}
			if (nCount = nSize >> BSF(PAGE_SIZE))
			{
				do
				{
					if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, PAGE_SIZE, NULL))
						goto READ1_FAILED;
					if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, PAGE_SIZE, NULL))
						goto READ2_FAILED;
					if (*lpiResult = memcmp(lpBuffer1, lpBuffer2, PAGE_SIZE))
						goto SUCCESS;
					(LPBYTE)lpAddress1 += PAGE_SIZE;
					(LPBYTE)lpAddress2 += PAGE_SIZE;
				} while (--nCount);
			}
			if (nSize &= PAGE_SIZE - 1)
			{
				if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, nSize, NULL))
					goto READ1_FAILED;
				if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, nSize, NULL))
					goto READ2_FAILED;
				*lpiResult = memcmp(lpBuffer1, lpBuffer2, nSize);
			}
		}
		else
		{
			if (IsBadReadPtr(lpAddress2, nSize))
				goto READ2_FAILED;
			if (nAlign = -(ptrdiff_t)lpAddress1 & (PAGE_SIZE - 1))
			{
				if (nAlign > nSize)
					nAlign = nSize;
				if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, nAlign, NULL))
					goto READ1_FAILED;
				if (*lpiResult = memcmp(lpBuffer1, lpAddress2, nAlign))
					goto SUCCESS;
				if (!(nSize -= nAlign))
					goto SUCCESS;
				(LPBYTE)lpAddress1 += nAlign;
				(LPBYTE)lpAddress2 += nAlign;
			}
			if (nCount = nSize >> BSF(PAGE_SIZE))
			{
				do
				{
					if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, PAGE_SIZE, NULL))
						goto READ1_FAILED;
					if (*lpiResult = memcmp(lpBuffer1, lpAddress2, PAGE_SIZE))
						goto SUCCESS;
					(LPBYTE)lpAddress1 += PAGE_SIZE;
					(LPBYTE)lpAddress2 += PAGE_SIZE;
				} while (--nCount);
			}
			if (nSize &= PAGE_SIZE - 1)
			{
				if (!ReadProcessMemory(hProcess1, lpAddress1, lpBuffer1, nSize, NULL))
					goto READ1_FAILED;
				*lpiResult = memcmp(lpBuffer1, lpAddress2, nSize);
			}
		}
	}
	else
	{
		if (hProcess2)
		{
			if (IsBadReadPtr(lpAddress1, nSize))
				goto READ1_FAILED;
			if (nAlign = -(ptrdiff_t)lpAddress2 & (PAGE_SIZE - 1))
			{
				if (nAlign > nSize)
					nAlign = nSize;
				if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, nAlign, NULL))
					goto READ2_FAILED;
				if (*lpiResult = memcmp(lpAddress1, lpBuffer2, nAlign))
					goto SUCCESS;
				if (!(nSize -= nAlign))
					goto SUCCESS;
				(LPBYTE)lpAddress1 += nAlign;
				(LPBYTE)lpAddress2 += nAlign;
			}
			if (nCount = nSize >> BSF(PAGE_SIZE))
			{
				do
				{
					if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, PAGE_SIZE, NULL))
						goto READ2_FAILED;
					if (*lpiResult = memcmp(lpAddress1, lpBuffer2, PAGE_SIZE))
						goto SUCCESS;
					(LPBYTE)lpAddress1 += PAGE_SIZE;
					(LPBYTE)lpAddress2 += PAGE_SIZE;
				} while (--nCount);
			}
			if (nSize &= PAGE_SIZE - 1)
			{
				if (!ReadProcessMemory(hProcess2, lpAddress2, lpBuffer2, nSize, NULL))
					goto READ2_FAILED;
				*lpiResult = memcmp(lpAddress1, lpBuffer2, nSize);
			}
		}
		else
		{
			if (IsBadReadPtr(lpAddress1, nSize))
				goto READ1_FAILED;
			if (IsBadReadPtr(lpAddress2, nSize))
				goto READ2_FAILED;
			*lpiResult = memcmp(lpAddress1, lpAddress2, nSize);
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
}
