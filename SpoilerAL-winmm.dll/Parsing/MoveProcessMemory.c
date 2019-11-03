#include "MoveProcessMemory.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "PageSize.h"

#ifdef __BORLANDC__
#pragma warn -8060
DWORD __stdcall GetProcessId(IN HANDLE Process);
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma function(memcpy, memmove)
#endif

static NTSTATUS __stdcall InternalMoveProcessMemory(
	IN          BOOL    bHandleOverlapRegions,
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
	IN          BOOL    bHandleOverlapRegions,
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize)
{
	__declspec(align(16)) BYTE lpBuffer[PAGE_SIZE];
	DWORD                      dwCurrentPID, dwSrcPID, dwDestPID;
	size_t                     nBytes;

	if (!nSize)
		goto SUCCESS;
	dwDestPID = dwSrcPID = dwCurrentPID = GetCurrentProcessId();
	if (hSrcProcess && !(dwSrcPID = GetProcessId(hSrcProcess)))
		goto READ_FAILED;
	if (hDestProcess && !(dwDestPID = GetProcessId(hDestProcess)))
		goto WRITE_FAILED;
	if (dwDestPID != dwCurrentPID)
	{
		if (dwSrcPID != dwCurrentPID)
		{
			BOOLEAN bOverlap;
			size_t  nStep;

			bOverlap = FALSE;
			if (dwDestPID == dwSrcPID)
			{
				if (lpDest == lpSrc)
					goto SUCCESS;
				if (!_sub_uintptr((size_t)lpDest, (size_t)lpSrc, &nStep))
					bOverlap = lpDest < (LPVOID)((LPBYTE)lpSrc + nSize);
			}
			if (!bOverlap)
			{
				if (nBytes = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nBytes > nSize)
						nBytes = nSize;
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nBytes, NULL))
						goto READ_FAILED;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nBytes, NULL))
						goto WRITE_FAILED;
				}
				while (nSize -= nBytes)
				{
					(LPBYTE)lpSrc += nBytes;
					(LPBYTE)lpDest += nBytes;
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nBytes = min(PAGE_SIZE, nSize), NULL))
						goto READ_FAILED;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nBytes, NULL))
						goto WRITE_FAILED;
				}
			}
			else if (bHandleOverlapRegions)
			{
				(LPBYTE)lpSrc += nSize;
				(LPBYTE)lpDest += nSize;
				if (nBytes = (size_t)lpDest & (PAGE_SIZE - 1))
				{
					if (nBytes > nSize)
						nBytes = nSize;
					if (!ReadProcessMemory(hSrcProcess, (LPBYTE)lpSrc -= nBytes, lpBuffer, nBytes, NULL))
						goto READ_FAILED;
					if (!WriteProcessMemory(hDestProcess, (LPBYTE)lpDest -= nBytes, lpBuffer, nBytes, NULL))
						goto WRITE_FAILED;
				}
				while (nSize -= nBytes)
				{
					(LPBYTE)lpSrc -= nBytes;
					(LPBYTE)lpDest -= nBytes;
					if (!ReadProcessMemory(hSrcProcess, (LPBYTE)lpSrc, lpBuffer, nBytes = min(PAGE_SIZE, nSize), NULL))
						goto READ_FAILED;
					if (!WriteProcessMemory(hDestProcess, (LPBYTE)lpDest, lpBuffer, nBytes, NULL))
						goto WRITE_FAILED;
				}
			}
			else
			{
				size_t nCount;

				if (nCount = nStep >> BSF_PAGE_SIZE)
				{
					nSize--;
					do
					{
						LPBYTE lpAddress;

						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, PAGE_SIZE, NULL))
							goto READ_FAILED;
						nBytes = nSize;
						nSize -= PAGE_SIZE;
						lpAddress = (LPBYTE)lpDest;
						(LPBYTE)lpSrc += PAGE_SIZE;
						(LPBYTE)lpDest += PAGE_SIZE;
						do
						{
							if (!WriteProcessMemory(hDestProcess, lpAddress, lpBuffer, PAGE_SIZE, NULL))
								goto WRITE_FAILED;
							lpAddress += nStep;
						} while (!_sub_uintptr(nBytes, nStep, &nBytes));
					} while (--nCount);
					nSize++;
				}
				if (nBytes = nStep & (PAGE_SIZE - 1))
				{
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nBytes, NULL))
						goto READ_FAILED;
					while (!_sub_uintptr(nSize, nStep, &nSize))
					{
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nBytes, NULL))
							goto WRITE_FAILED;
						(LPBYTE)lpDest += nStep;
					}
					if ((nSize += nStep) && !WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
						goto WRITE_FAILED;
				}
			}
		}
		else
		{
			if (IsBadReadPtr(lpSrc, nSize))
				goto READ_FAILED;
			if (nBytes = -(ptrdiff_t)lpDest & (PAGE_SIZE - 1))
			{
				if (nBytes > nSize)
					nBytes = nSize;
				if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, nBytes, NULL))
					goto WRITE_FAILED;
			}
			while (nSize -= nBytes)
			{
				(LPBYTE)lpSrc += nBytes;
				(LPBYTE)lpDest += nBytes;
				if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, nBytes = min(PAGE_SIZE, nSize), NULL))
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
			if (nBytes = -(ptrdiff_t)lpSrc & (PAGE_SIZE - 1))
			{
				if (nBytes > nSize)
					nBytes = nSize;
				if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, nBytes, NULL))
					goto READ_FAILED;
			}
			while (nSize -= nBytes)
			{
				(LPBYTE)lpSrc += nBytes;
				(LPBYTE)lpDest += nBytes;
				if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, nBytes = min(PAGE_SIZE, nSize), NULL))
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

READ_FAILED:
	return STATUS_MEMORY_READ_FAILED;

WRITE_FAILED:
	return STATUS_MEMORY_WRITE_FAILED;
}
