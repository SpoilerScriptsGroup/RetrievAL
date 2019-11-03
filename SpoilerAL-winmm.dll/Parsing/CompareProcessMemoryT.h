#ifndef _DEBUG
#include "noinline_wchar.h"
#endif
#include "CompareProcessMemory.h"
#include "intrinsic.h"
#include "PageSize.h"
#include "IsBadPtr.h"
#include <tchar.h>

#ifdef _UNICODE
#define CompareProcessMemoryT CompareProcessMemoryW
extern int __cdecl _wmemicmp(const void *buffer1, const void *buffer2, size_t count);
#define _tmemcmp wmemcmp
#define _tmemicmp _wmemicmp
#else
#define CompareProcessMemoryT CompareProcessMemoryA
#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma function(memcmp)
#endif
#define _tmemcmp memcmp
#define _tmemicmp _memicmp
#endif

#ifdef _DEBUG
#include <assert.h>
# define ASSERT assert
#else
# define ASSERT __assume
#endif

#ifdef __BORLANDC__
#pragma warn -8060
#undef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
EXTERN_C DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

NTSTATUS __stdcall CompareProcessMemoryT(
	OUT         int     *lpiResult,
	IN OPTIONAL HANDLE  hProcess1,
	IN          LPCVOID lpAddress1,
	IN OPTIONAL HANDLE  hProcess2,
	IN          LPCVOID lpAddress2,
	IN          size_t  nCount,
	IN          BOOL    bInsensitiveCase)
{
	__declspec(align(16)) BYTE buffer1[PAGE_SIZE];
	__declspec(align(16)) BYTE buffer2[PAGE_SIZE];
	BOOLEAN                    isSameProcess;
	size_t                     size, read;
	int(__cdecl *lpComparator)(const void *buffer1, const void *buffer2, size_t count);

	*lpiResult = 0;
	if (!nCount)
		goto SUCCESS;
	lpComparator = !bInsensitiveCase ? _tmemcmp : _tmemicmp;
	if (hProcess1 != hProcess2)
	{
		DWORD dwCurPID, dwPID1, dwPID2;

		dwPID2 = dwPID1 = dwCurPID = GetCurrentProcessId();
		if (hProcess1)
		{
			dwPID1 = GetProcessId(hProcess1);
			if (!dwPID1)
				goto READ1_FAILED;
			if (dwPID1 == dwCurPID)
				hProcess1 = NULL;
		}
		if (hProcess2)
		{
			dwPID2 = GetProcessId(hProcess2);
			if (!dwPID2)
				goto READ2_FAILED;
			if (dwPID2 == dwCurPID)
				hProcess2 = NULL;
		}
		isSameProcess = dwPID1 == dwPID2;
	}
	else
	{
		if (hProcess1)
		{
			DWORD dwProcessId;

			dwProcessId = GetProcessId(hProcess1);
			if (!dwProcessId)
				goto READ1_FAILED;
			if (dwProcessId == GetCurrentProcessId())
				hProcess2 = hProcess1 = NULL;
		}
		isSameProcess = TRUE;
	}
	if (isSameProcess && lpAddress1 == lpAddress2)
		goto SUCCESS;
#ifdef _UNICODE
	size = min(nCount, SIZE_MAX / sizeof(TCHAR)) * sizeof(TCHAR);
#else
	size = nCount;
#endif
	if (hProcess1)
	{
		if (hProcess2)
		{
			size_t read1, read2, compare, offset;

			read1 = min(PAGE_SIZE - ((size_t)lpAddress1 & (PAGE_SIZE - sizeof(TCHAR))), size);
			ASSERT(read1 <= PAGE_SIZE);
			if (!ReadProcessMemory(hProcess1, lpAddress1, buffer1, read1, NULL))
				goto READ1_FAILED;
			read2 = min(PAGE_SIZE - ((size_t)lpAddress2 & (PAGE_SIZE - sizeof(TCHAR))), size);
			ASSERT(read2 <= PAGE_SIZE);
			if (!ReadProcessMemory(hProcess2, lpAddress2, buffer2, read2, NULL))
				goto READ2_FAILED;
			offset = compare = min(read1, read2);
			if (*lpiResult = lpComparator((TCHAR *)buffer1, (TCHAR *)buffer2, compare / sizeof(TCHAR)))
				goto SUCCESS;
			if (!(size -= compare))
				goto SUCCESS;
			(LPBYTE)lpAddress1 += read1;
			(LPBYTE)lpAddress2 += read2;
			if (_sub_uintptr(read2, read1, &compare))
			{
				compare = -(ptrdiff_t)compare;
				goto READ_BUFFER2;
			}
			for (; ; )
			{
				if (!ReadProcessMemory(hProcess1, lpAddress1, buffer1, min(PAGE_SIZE, size), NULL))
					goto READ1_FAILED;
				if (compare)
				{
					compare = min(compare, size);
					ASSERT(compare <= PAGE_SIZE);
					if (*lpiResult = lpComparator((TCHAR *)buffer1, (TCHAR *)(buffer2 + offset), compare / sizeof(TCHAR)))
						break;
					if (!(size -= compare))
						break;
				}
				(LPBYTE)lpAddress1 += PAGE_SIZE;
				compare = PAGE_SIZE - (offset = compare);
			READ_BUFFER2:
				if (!ReadProcessMemory(hProcess2, lpAddress2, buffer2, min(PAGE_SIZE, size), NULL))
					goto READ2_FAILED;
				compare = min(compare, size);
				ASSERT(compare <= PAGE_SIZE);
				if (*lpiResult = lpComparator((TCHAR *)(buffer1 + offset), (TCHAR *)buffer2, compare / sizeof(TCHAR)))
					break;
				if (!(size -= compare))
					break;
				(LPBYTE)lpAddress2 += PAGE_SIZE;
				compare = PAGE_SIZE - (offset = compare);
			}
		}
		else
		{
			if (IsBadReadPtr(lpAddress2, size))
				goto READ2_FAILED;
			if (read = -(ptrdiff_t)lpAddress1 & (PAGE_SIZE - sizeof(TCHAR)))
			{
				read = min(read, size);
				ASSERT(read < PAGE_SIZE);
				if (!ReadProcessMemory(hProcess1, lpAddress1, buffer1, read, NULL))
					goto READ1_FAILED;
				if (*lpiResult = lpComparator((TCHAR *)buffer1, lpAddress2, read / sizeof(TCHAR)))
					goto SUCCESS;
			}
			while (size -= read)
			{
				(LPBYTE)lpAddress1 += read;
				(LPBYTE)lpAddress2 += read;
				if (!ReadProcessMemory(hProcess1, lpAddress1, buffer1, read = min(PAGE_SIZE, size), NULL))
					goto READ1_FAILED;
				if (*lpiResult = lpComparator((TCHAR *)buffer1, lpAddress2, read / sizeof(TCHAR)))
					break;
			}
		}
	}
	else
	{
		if (hProcess2)
		{
			if (IsBadReadPtr(lpAddress1, size))
				goto READ1_FAILED;
			if (read = -(ptrdiff_t)lpAddress2 & (PAGE_SIZE - sizeof(TCHAR)))
			{
				read = min(read, size);
				ASSERT(read < PAGE_SIZE);
				if (!ReadProcessMemory(hProcess2, lpAddress2, buffer2, read, NULL))
					goto READ2_FAILED;
				if (*lpiResult = lpComparator(lpAddress1, (TCHAR *)buffer2, read / sizeof(TCHAR)))
					goto SUCCESS;
			}
			while (size -= read)
			{
				(LPBYTE)lpAddress1 += read;
				(LPBYTE)lpAddress2 += read;
				if (!ReadProcessMemory(hProcess2, lpAddress2, buffer2, read = min(PAGE_SIZE, size), NULL))
					goto READ2_FAILED;
				if (*lpiResult = lpComparator(lpAddress1, (TCHAR *)buffer2, read / sizeof(TCHAR)))
					break;
			}
		}
		else
		{
			if (IsBadReadPtr(lpAddress1, size))
				goto READ1_FAILED;
			if (IsBadReadPtr(lpAddress2, size))
				goto READ2_FAILED;
			*lpiResult = lpComparator(lpAddress1, lpAddress2, nCount);
		}
	}

SUCCESS:
	return STATUS_SUCCESS;

READ1_FAILED:
	return STATUS_MEMORY_READ1_FAILED;

READ2_FAILED:
	return STATUS_MEMORY_READ2_FAILED;
}
