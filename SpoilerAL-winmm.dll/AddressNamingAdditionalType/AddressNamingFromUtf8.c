#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "TranscodeMultiByte.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

void __fastcall AddressNamingFromUtf8(unsigned long DataSize, char *tmpC, vector_string* tmpV)
{
#ifdef _NO_CRT_STDIO_INLINE
	extern HANDLE hHeap;
	LPWSTR lpWideCharStr;
	string* format = &vector_type_at(tmpV, string, 5);
	if (lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (DataSize + 1) * sizeof(wchar_t))) {
		MultiByteToWideChar(CP_UTF8, 0, tmpC, -1, lpWideCharStr, DataSize + 1);
		lpWideCharStr[DataSize] = L'\0';
		_snprintf(tmpC, DataSize + 1, string_empty(format) ? "%S" : string_c_str(format), lpWideCharStr);
		HeapFree(hHeap, 0, lpWideCharStr);
	} else {
		*tmpC = '\0';
	}
	string_clear(format);
#else
	Utf8ToMultiByte(CP_THREAD_ACP, 0, tmpC, -1, tmpC, DataSize + 1, NULL, NULL);
#endif
}
