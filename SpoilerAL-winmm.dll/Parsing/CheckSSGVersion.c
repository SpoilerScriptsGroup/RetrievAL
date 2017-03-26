#include <windows.h>
#include "intrinsic.h"

extern HANDLE hHeap;

#define SSGVERSION_LENGTH 3
const LPSTR lpSSGVersion = "6.2";

BOOL EnableParserFix = TRUE;

EXTERN_C void __stdcall CheckSSGVersion(LPSTR lpFileName)
{
	HANDLE hFile;

	EnableParserFix = FALSE;
	hFile = CreateFileA(
		lpFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do	// do { ... } while (0);
		{
			DWORD dwFileSize;
			char  lpIdentify[21];
			DWORD nBytesRead;
			DWORD dwLength;
			char  *lpBuffer;

			dwFileSize = GetFileSize(hFile, NULL);
			if (dwFileSize == MAXDWORD)
				break;
			if (dwFileSize < 21 + SSGVERSION_LENGTH)
				break;
			if (!ReadFile(hFile, lpIdentify, 21, &nBytesRead, NULL))
				break;
			if (nBytesRead != 21)
				break;
			if (*(LPDWORD) lpIdentify       != BSWAP32('SSG ') ||
				*(LPDWORD)(lpIdentify +  4) != BSWAP32('for ') ||
				*(LPDWORD)(lpIdentify +  8) != BSWAP32('Spoi') ||
				*(LPDWORD)(lpIdentify + 12) != BSWAP32('lerA') ||
				*(LPDWORD)(lpIdentify + 16) != BSWAP32('L ve') ||
				*(LPBYTE )(lpIdentify + 20) !=   (BYTE)'r')
			{
				break;
			}
			dwLength = dwFileSize - 21;
			lpBuffer = (char *)HeapAlloc(hHeap, 0, dwLength + 1);
			if (lpBuffer == NULL)
				break;
			do	// do { ... } while (0);
			{
				char *p;

				if (!ReadFile(hFile, lpBuffer, dwLength, &nBytesRead, NULL))
					break;
				if (nBytesRead != dwLength)
					break;
				lpBuffer[dwLength] = '\0';
				p = lpBuffer;
				while (__intrinsic_isspace(*p))
				{
					if (--dwLength < SSGVERSION_LENGTH)
						goto NESTED_BREAK;
					p++;
				}
				if (strncmp(p, lpSSGVersion, SSGVERSION_LENGTH + 1) < 0)
					break;
				EnableParserFix = TRUE;
			} while (0);
		NESTED_BREAK:
			HeapFree(hHeap, 0, lpBuffer);
		} while (0);
		CloseHandle(hFile);
	}
}
