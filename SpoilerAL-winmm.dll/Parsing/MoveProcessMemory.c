#include "MoveProcessMemory.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "PageSize.h"

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

static NTSTATUS __stdcall InternalMoveProcessMemory(
	IN          BOOLEAN bHandleOverlapRegions,
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize);

__declspec(naked) NTSTATUS __stdcall CopyProcessMemory(
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize)
{
	__asm
	{
		pop     eax
		push    FALSE
		push    eax
		jmp     InternalMoveProcessMemory
	}
}

__declspec(naked) NTSTATUS __stdcall MoveProcessMemory(
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize)
{
	__asm
	{
		pop     eax
		push    TRUE
		push    eax
		jmp     InternalMoveProcessMemory
	}
}

static NTSTATUS __stdcall InternalMoveProcessMemory(
	IN          BOOLEAN bHandleOverlapRegions,
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize)
{
	BYTE   lpBuffer[PAGE_SIZE];
	DWORD  dwCurrentPID, dwSrcPID, dwDestPID;
	size_t nAlign, nCount;

	if (!nSize)
		goto SUCCESS;
	dwDestPID = dwSrcPID = dwCurrentPID = GetCurrentProcessId();
	if (hSrcProcess && !(dwSrcPID = GetProcessId(hSrcProcess)))
		goto ACCESS_DENIED;
	if (hDestProcess && !(dwDestPID = GetProcessId(hDestProcess)))
		goto ACCESS_DENIED;
	if (dwDestPID != dwCurrentPID)
	{
		if (dwSrcPID != dwCurrentPID)
		{
			if (dwDestPID == dwSrcPID && lpDest == lpSrc)
				goto SUCCESS;
			if (dwDestPID != dwSrcPID || lpDest <= lpSrc || lpDest >= (LPVOID)((LPBYTE)lpSrc + nSize))
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
				if (nCount = nSize >> BSF(PAGE_SIZE))
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
			else if (bHandleOverlapRegions)
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
				if (nCount = nSize >> BSF(PAGE_SIZE))
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
			else
			{
				BYTE c;

				do
				{
					if (!ReadProcessMemory(hSrcProcess, ((LPBYTE)lpSrc)++, &c, 1, NULL))
						goto READ_FAILED;
					if (!WriteProcessMemory(hDestProcess, ((LPBYTE)lpDest)++, &c, 1, NULL))
						goto WRITE_FAILED;
				} while (--nSize);
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
			if (nCount = nSize >> BSF(PAGE_SIZE))
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
		if (dwSrcPID != dwCurrentPID)
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
			if (nCount = nSize >> BSF(PAGE_SIZE))
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
			if (lpDest == lpSrc)
				goto SUCCESS;
			if (IsBadReadPtr(lpSrc, nSize))
				goto READ_FAILED;
			if (IsBadWritePtr(lpDest, nSize))
				goto WRITE_FAILED;
			if (lpDest <= lpSrc || lpDest >= (LPVOID)((LPBYTE)lpSrc + nSize))
				memcpy(lpDest, lpSrc, nSize);
			else if (bHandleOverlapRegions)
				memmove(lpDest, lpSrc, nSize);
			else
				__movsb(lpDest, lpSrc, nSize);
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
