#include "TranscodeMultiByte.h"

#if defined(__BORLANDC__)
#pragma warn -8060
#endif

extern HANDLE hHeap;

int __stdcall TranscodeMultiByte(
	UINT   uSrcCodePage,
	DWORD  dwSrcFlags,
	LPCSTR lpSrcStr,
	int    cchSrcLength,
	UINT   uDestCodePage,
	DWORD  dwDestFlags,
	LPSTR  lpDestStr,
	int    cchDestLength,
	LPCSTR lpDefaultChar,
	LPBOOL lpUsedDefaultChar)
{
	int cchLength;

	if (cchLength = MultiByteToWideChar(uSrcCodePage, dwSrcFlags, lpSrcStr, cchSrcLength, NULL, 0))
	{
		LPWSTR lpWideCharStr;

		if (lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchLength * sizeof(wchar_t)))
		{
			MultiByteToWideChar(uSrcCodePage, dwSrcFlags, lpSrcStr, cchSrcLength, lpWideCharStr, cchLength);
			cchLength = WideCharToMultiByte(uDestCodePage, dwDestFlags, lpWideCharStr, cchLength, lpDestStr, cchDestLength, lpDefaultChar, lpUsedDefaultChar);
			HeapFree(hHeap, 0, lpWideCharStr);
		}
		else
		{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			cchLength = 0;
		}
	}
	return cchLength;
}
