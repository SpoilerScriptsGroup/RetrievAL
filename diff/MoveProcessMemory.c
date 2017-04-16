#include "MoveProcessMemory.h"
#include "IsBadXxxPtr.h"

#ifdef __BORLANDC__
#pragma warn -8060
#endif

NTSTATUS __stdcall MoveProcessMemory(
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize)
{
	BYTE   lpBuffer[4096];
	size_t nAlign;
	size_t nCount;

	if (!nSize)
		goto SUCCESS;
	if (hDestProcess)
	{
		if (hSrcProcess)
		{
			if (lpDest == lpSrc)
				goto SUCCESS;
			if (lpDest <= lpSrc || lpDest >= (LPVOID)((LPBYTE)lpSrc + nSize))
			{
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
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
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, sizeof(lpBuffer), NULL))
							goto READ_FAILED;
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							goto WRITE_FAILED;
						(LPBYTE)lpSrc += sizeof(lpBuffer);
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
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
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
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
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						(LPBYTE)lpSrc -= sizeof(lpBuffer);
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, sizeof(lpBuffer), NULL))
							goto READ_FAILED;
						(LPBYTE)lpDest -= sizeof(lpBuffer);
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
							goto WRITE_FAILED;
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
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
			if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
			{
				nAlign = sizeof(lpBuffer) - nAlign;
				if (nAlign > nSize)
					nAlign = nSize;
				if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, nAlign, NULL))
					goto WRITE_FAILED;
				if (!(nSize -= nAlign))
					goto SUCCESS;
				(LPBYTE)lpSrc += nAlign;
				(LPBYTE)lpDest += nAlign;
			}
			if (nCount = nSize / sizeof(lpBuffer))
			{
				do
				{
					if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, sizeof(lpBuffer), NULL))
						goto WRITE_FAILED;
					(LPBYTE)lpSrc += sizeof(lpBuffer);
					(LPBYTE)lpDest += sizeof(lpBuffer);
				} while (--nCount);
			}
			if (nSize %= sizeof(lpBuffer))
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
			if (nAlign = (size_t)lpSrc % sizeof(lpBuffer))
			{
				nAlign = sizeof(lpBuffer) - nAlign;
				if (nAlign > nSize)
					nAlign = nSize;
				if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, nAlign, NULL))
					goto READ_FAILED;
				if (!(nSize -= nAlign))
					goto SUCCESS;
				(LPBYTE)lpSrc += nAlign;
				(LPBYTE)lpDest += nAlign;
			}
			if (nCount = nSize / sizeof(lpBuffer))
			{
				do
				{
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, sizeof(lpBuffer), NULL))
						goto READ_FAILED;
					(LPBYTE)lpSrc += sizeof(lpBuffer);
					(LPBYTE)lpDest += sizeof(lpBuffer);
				} while (--nCount);
			}
			if (nSize %= sizeof(lpBuffer))
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
			memmove(lpDest, lpSrc, nSize);
		}
	}

SUCCESS:
	return STATUS_SUCCESS;

READ_FAILED:
	return STATUS_MEMORY_READ_FAILED;

WRITE_FAILED:
	return STATUS_MEMORY_WRITE_FAILED;
}
