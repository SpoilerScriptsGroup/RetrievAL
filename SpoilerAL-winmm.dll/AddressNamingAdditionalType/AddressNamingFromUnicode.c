#include <windows.h>
#include <intrin.h>

extern HANDLE hHeap;

EXTERN_C void __stdcall AddressNamingFromUnicode(unsigned long DataSize, char *tmpC)
{
	DWORD dwBytes = DataSize + 2;
	LPWSTR lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, dwBytes);
	if (lpWideCharStr != NULL)
	{
		DWORD dwCount = (dwBytes >> 1) - 1;
		lpWideCharStr[dwCount] = L'\0';
		__movsw((unsigned short *)lpWideCharStr, (const unsigned short *)tmpC, dwCount);
		WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, tmpC, DataSize + 1, NULL, NULL);
		HeapFree(hHeap, 0, lpWideCharStr);
	}
	else
	{
		*tmpC = '\0';
	}
}
