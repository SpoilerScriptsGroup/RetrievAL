#include "MoveProcessMemory.h"
#include "IsBadPtr.h"
#include "PageSize.h"

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

NTSTATUS __stdcall MoveProcessMemory(
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize)
{
	BYTE    lpBuffer[PAGE_SIZE];
	BOOLEAN bIsSameProcess;
	size_t  nAlign;
	size_t  nCount;

	if (!nSize)
		goto SUCCESS;
	if (hSrcProcess != hDestProcess)
	{
		DWORD dwCurPID, dwSrcPID, dwDestPID;

		dwDestPID = dwSrcPID = dwCurPID = GetCurrentProcessId();
		if (hSrcProcess)
		{
			dwSrcPID = GetProcessId(hSrcProcess);
			if (!dwSrcPID)
				goto ACCESS_DENIED;
			if (dwSrcPID == dwCurPID)
				hSrcProcess = NULL;
		}
		if (hDestProcess)
		{
			dwDestPID = GetProcessId(hDestProcess);
			if (!dwDestPID)
				goto ACCESS_DENIED;
			if (dwDestPID == dwCurPID)
				hDestProcess = NULL;
		}
		bIsSameProcess = dwDestPID == dwSrcPID;
	}
	else
	{
		if (hSrcProcess)
		{
			DWORD dwProcessId;

			dwProcessId = GetProcessId(hSrcProcess);
			if (!dwProcessId)
				goto ACCESS_DENIED;
			if (dwProcessId == GetCurrentProcessId())
				hDestProcess = hSrcProcess = NULL;
		}
		bIsSameProcess = TRUE;
	}
	if (bIsSameProcess && lpDest == lpSrc)
		goto SUCCESS;
	if (hDestProcess)
	{
		if (hSrcProcess)
		{
			if (!bIsSameProcess || lpDest <= lpSrc || lpDest >= (LPVOID)((LPBYTE)lpSrc + nSize))
			{
				if (nAlign = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nAlign > nSize)
						nAlign = nSize;
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nAlign, NULL))
						goto READ_FAILED;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nAlign, NULL))
						goto WRITE_FAILED;
					if (!(nSize -= nAlign))
						goto SUCCESS;
					(LPBYTE)lpSrc += nAlign;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / PAGE_SIZE)
				{
					do
					{
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, PAGE_SIZE, NULL))
							goto READ_FAILED;
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, PAGE_SIZE, NULL))
							goto WRITE_FAILED;
						(LPBYTE)lpSrc += PAGE_SIZE;
						(LPBYTE)lpDest += PAGE_SIZE;
					} while (--nCount);
				}
				if (nSize &= PAGE_SIZE - 1)
				{
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nSize, NULL))
						goto READ_FAILED;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
						goto WRITE_FAILED;
				}
			}
			else
			{
				(LPBYTE)lpSrc += nSize;
				(LPBYTE)lpDest += nSize;
				if (nAlign = (size_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nAlign > nSize)
						nAlign = nSize;
					(LPBYTE)lpSrc -= nAlign;
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nAlign, NULL))
						goto READ_FAILED;
					(LPBYTE)lpDest -= nAlign;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nAlign, NULL))
						goto WRITE_FAILED;
					if (!(nSize -= nAlign))
						goto SUCCESS;
				}
				if (nCount = nSize / PAGE_SIZE)
				{
					do
					{
						(LPBYTE)lpSrc -= PAGE_SIZE;
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, PAGE_SIZE, NULL))
							goto READ_FAILED;
						(LPBYTE)lpDest -= PAGE_SIZE;
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, PAGE_SIZE, NULL))
							goto WRITE_FAILED;
					} while (--nCount);
				}
				if (nSize &= PAGE_SIZE - 1)
				{
					(LPBYTE)lpSrc -= nSize;
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nSize, NULL))
						goto READ_FAILED;
					(LPBYTE)lpDest -= nSize;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
						goto WRITE_FAILED;
				}
			}
		}
		else
		{
			if (IsBadReadPtr(lpSrc, nSize))
				goto READ_FAILED;
			if (nAlign = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
			{
				if (nAlign > nSize)
					nAlign = nSize;
				if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, nAlign, NULL))
					goto WRITE_FAILED;
				if (!(nSize -= nAlign))
					goto SUCCESS;
				(LPBYTE)lpSrc += nAlign;
				(LPBYTE)lpDest += nAlign;
			}
			if (nCount = nSize / PAGE_SIZE)
			{
				do
				{
					if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, PAGE_SIZE, NULL))
						goto WRITE_FAILED;
					(LPBYTE)lpSrc += PAGE_SIZE;
					(LPBYTE)lpDest += PAGE_SIZE;
				} while (--nCount);
			}
			if (nSize &= PAGE_SIZE - 1)
			{
				if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, nSize, NULL))
					goto WRITE_FAILED;
			}
		}
	}
	else
	{
		if (hSrcProcess)
		{
			if (IsBadWritePtr(lpDest, nSize))
				goto WRITE_FAILED;
			if (nAlign = -(ptrdiff_t)lpSrc & (PAGE_SIZE - 1))
			{
				if (nAlign > nSize)
					nAlign = nSize;
				if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, nAlign, NULL))
					goto READ_FAILED;
				if (!(nSize -= nAlign))
					goto SUCCESS;
				(LPBYTE)lpSrc += nAlign;
				(LPBYTE)lpDest += nAlign;
			}
			if (nCount = nSize / PAGE_SIZE)
			{
				do
				{
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, PAGE_SIZE, NULL))
						goto READ_FAILED;
					(LPBYTE)lpSrc += PAGE_SIZE;
					(LPBYTE)lpDest += PAGE_SIZE;
				} while (--nCount);
			}
			if (nSize &= PAGE_SIZE - 1)
			{
				if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, nSize, NULL))
					goto READ_FAILED;
			}
		}
		else
		{
			if (IsBadReadPtr(lpSrc, nSize))
				goto READ_FAILED;
			if (IsBadWritePtr(lpDest, nSize))
				goto WRITE_FAILED;
			memmove(lpDest, lpSrc, nSize);
		}
	}

SUCCESS:
	return STATUS_SUCCESS;

ACCESS_DENIED:
	return STATUS_PROCESS_ACCESS_DENIED;

READ_FAILED:
	return STATUS_MEMORY_READ_FAILED;

WRITE_FAILED:
	return STATUS_MEMORY_WRITE_FAILED;
}
