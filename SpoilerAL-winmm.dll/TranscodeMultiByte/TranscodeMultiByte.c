#include "TranscodeMultiByte.h"

#if defined(__BORLANDC__)
#pragma warn -8060
#endif

extern HANDLE hHeap;

int __stdcall TranscodeMultiByte(
	UINT   uSrcCodePage,
	DWORD  dwSrcFlags,
	LPCSTR lpSrcStr,
	int    cbSrcLength,
	UINT   uDestCodePage,
	DWORD  dwDestFlags,
	LPSTR  lpDestStr,
	int    cbDestLength,
	LPCSTR lpDefaultChar,
	LPBOOL lpUsedDefaultChar)
{
	unsigned int length;

	if (length = MultiByteToWideChar(uSrcCodePage, dwSrcFlags, lpSrcStr, cbSrcLength, NULL, 0))
	{
		LPWSTR lpWideCharStr;

		if (lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)length * sizeof(wchar_t)))
		{
			MultiByteToWideChar(uSrcCodePage, dwSrcFlags, lpSrcStr, cbSrcLength, lpWideCharStr, length);
			length = WideCharToMultiByte(uDestCodePage, dwDestFlags, lpWideCharStr, length, lpDestStr, cbDestLength, lpDefaultChar, lpUsedDefaultChar);
			HeapFree(hHeap, 0, lpWideCharStr);
			return length;
		}
		else
		{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		}
	}
	return 0;
}
