#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8060
#endif

BOOLEAN __stdcall MoveProcessMemory(
	IN OPTIONAL HANDLE       hDestProcess,
	IN          void *       lpDest,
	IN OPTIONAL HANDLE       hSrcProcess,
	IN          const void * lpSrc,
	IN          size_t       nSize)
{
	do	/* do { ... } while (0); */
	{
		BYTE   lpBuffer[4096];
		size_t nAlign;
		size_t nCount;

		if (!nSize)
			break;
		if (hDestProcess)
		{
			if (hSrcProcess)
			{
				if (lpDest == lpSrc)
					break;
				if (lpDest <= lpSrc || lpDest >= (void *)((LPBYTE)lpSrc + nSize))
				{
					if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
					{
						nAlign = sizeof(lpBuffer) - nAlign;
						if (nAlign > nSize)
							nAlign = nSize;
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nAlign, NULL))
							return FALSE;
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nAlign, NULL))
							return FALSE;
						if (!(nSize -= nAlign))
							break;
						(LPBYTE)lpSrc += nAlign;
						(LPBYTE)lpDest += nAlign;
					}
					if (nCount = nSize / sizeof(lpBuffer))
					{
						do
						{
							if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, sizeof(lpBuffer), NULL))
								return FALSE;
							if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
								return FALSE;
							(LPBYTE)lpSrc += sizeof(lpBuffer);
							(LPBYTE)lpDest += sizeof(lpBuffer);
						} while (--nCount);
					}
					if (nSize %= sizeof(lpBuffer))
					{
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nSize, NULL))
							return FALSE;
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
							return FALSE;
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
							return FALSE;
						(LPBYTE)lpDest -= nAlign;
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nAlign, NULL))
							return FALSE;
						if (!(nSize -= nAlign))
							break;
					}
					if (nCount = nSize / sizeof(lpBuffer))
					{
						do
						{
							(LPBYTE)lpSrc -= sizeof(lpBuffer);
							if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, sizeof(lpBuffer), NULL))
								return FALSE;
							(LPBYTE)lpDest -= sizeof(lpBuffer);
							if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, sizeof(lpBuffer), NULL))
								return FALSE;
						} while (--nCount);
					}
					if (nSize %= sizeof(lpBuffer))
					{
						(LPBYTE)lpSrc -= nSize;
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpBuffer, nSize, NULL))
							return FALSE;
						(LPBYTE)lpDest -= nSize;
						if (!WriteProcessMemory(hDestProcess, lpDest, lpBuffer, nSize, NULL))
							return FALSE;
					}
				}
			}
			else
			{
				if (nAlign = (size_t)lpDest % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						break;
					(LPBYTE)lpSrc += nAlign;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpSrc += sizeof(lpBuffer);
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
				{
					if (!WriteProcessMemory(hDestProcess, lpDest, lpSrc, nSize, NULL))
						return FALSE;
				}
			}
		}
		else
		{
			if (hSrcProcess)
			{
				if (nAlign = (size_t)lpSrc % sizeof(lpBuffer))
				{
					nAlign = sizeof(lpBuffer) - nAlign;
					if (nAlign > nSize)
						nAlign = nSize;
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, nAlign, NULL))
						return FALSE;
					if (!(nSize -= nAlign))
						break;
					(LPBYTE)lpSrc += nAlign;
					(LPBYTE)lpDest += nAlign;
				}
				if (nCount = nSize / sizeof(lpBuffer))
				{
					do
					{
						if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, sizeof(lpBuffer), NULL))
							return FALSE;
						(LPBYTE)lpSrc += sizeof(lpBuffer);
						(LPBYTE)lpDest += sizeof(lpBuffer);
					} while (--nCount);
				}
				if (nSize %= sizeof(lpBuffer))
				{
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpDest, nSize, NULL))
						return FALSE;
				}
			}
			else
			{
				memmove(lpDest, lpSrc, nSize);
			}
		}
	} while (0);
	return TRUE;
}
