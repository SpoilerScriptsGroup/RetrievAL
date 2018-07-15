#define _CRT_SECURE_NO_WARNINGS
#ifdef _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#else
#include "TranscodeMultiByte.h"
#endif
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

extern HANDLE hHeap;

void __stdcall AddressNamingFromUtf8(unsigned long DataSize, char *tmpC, vector_string* tmpV)
{
#ifdef _CRT_SECURE_NO_WARNINGS
	LPWSTR lpWideCharStr;
	string* format = &vector_type_at(tmpV, string, 5);
	if (lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, DataSize * sizeof(wchar_t) + sizeof(wchar_t))) {
		MultiByteToWideChar(CP_UTF8, 0, tmpC, -1, lpWideCharStr, DataSize + 1);
		_snprintf(tmpC, DataSize + 1, string_empty(format) ? "%S" : string_c_str(format), lpWideCharStr);
		HeapFree(hHeap, 0, lpWideCharStr);
	} else *tmpC = '\0';
#else
	Utf8ToMultiByte(CP_THREAD_ACP, 0, tmpC, -1, tmpC, DataSize + 1, NULL, NULL);
#endif
}
