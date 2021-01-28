#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "TranscodeMultiByte.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

extern HANDLE hHeap;

void __fastcall AddressNamingFromUtf8(
	unsigned long  const DataSize,
	char          *const tmpC,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGCtrl,
	TSSGSubject   *const SSGS)
{
#ifdef _NO_CRT_STDIO_INLINE
	LPWSTR lpWideCharStr;
	string *const format = &vector_at(tmpV, 5);
	if (lpWideCharStr = HeapAlloc(hHeap, 0, (DataSize + 1) * sizeof(wchar_t)))
	{
		if (MultiByteToWideChar(CP_UTF8, 0, tmpC, -1, lpWideCharStr, DataSize + 1))
		{
			lpWideCharStr[DataSize] = L'\0';
			_snprintf(tmpC, DataSize + 1, string_empty(format) ? "%ls" : string_c_str(format), lpWideCharStr);
		}
		else
			*tmpC = '\0';
		HeapFree(hHeap, 0, lpWideCharStr);
	}
	else
	{
		*tmpC = '\0';
		SetLastError(ERROR_OUTOFMEMORY);
	}
	string_clear(format);
#else
	Utf8ToMultiByte(CP_THREAD_ACP, 0, tmpC, -1, tmpC, DataSize + 1, NULL, NULL);
#endif
}
