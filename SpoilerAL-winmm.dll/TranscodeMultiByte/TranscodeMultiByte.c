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
			SetLastError(ERROR_OUTOFMEMORY);
		}
	}
	return 0;
}

#undef MultiByteToUtf8
#ifndef _M_IX86
int __stdcall MultiByteToUtf8(
	UINT   CodePage,
	DWORD  dwFlags,
	LPCSTR lpMultiByteStr,
	int    cbMultiByte,
	LPSTR  lpUtf8Str,
	int    cbUtf8)
{
	return
		TranscodeMultiByte(
			CodePage,
			dwFlags,
			lpMultiByteStr,
			cbMultiByte,
			CP_UTF8,
			0,
			lpUtf8Str,
			cbUtf8,
			NULL,
			NULL);
}
#else
__declspec(naked) int __stdcall MultiByteToUtf8(
	UINT   CodePage,
	DWORD  dwFlags,
	LPCSTR lpMultiByteStr,
	int    cbMultiByte,
	LPSTR  lpUtf8Str,
	int    cbUtf8)
{
	__asm
	{
		#define ReturnAddress  esp
		#define CodePage       (esp + 4)
		#define dwFlags        (esp + 8)
		#define lpMultiByteStr (esp + 12)
		#define cbMultiByte    (esp + 16)
		#define lpUtf8Str      (esp + 20)
		#define cbUtf8         (esp + 24)

		mov     eax, dword ptr [lpMultiByteStr]
		mov     ecx, dword ptr [dwFlags]
		push    eax
		push    ecx
		mov     eax, dword ptr [CodePage + 8]
		mov     ecx, dword ptr [ReturnAddress + 8]
		push    eax
		push    ecx
		mov     eax, dword ptr [cbMultiByte + 16]
		mov     ecx, dword ptr [lpUtf8Str + 16]
		mov     edx, dword ptr [cbUtf8 + 16]
		mov     dword ptr [esp + 16], eax
		mov     dword ptr [esp + 20], CP_UTF8
		mov     dword ptr [esp + 24], 0
		mov     dword ptr [esp + 28], ecx
		mov     dword ptr [esp + 32], edx
		mov     dword ptr [esp + 36], 0
		mov     dword ptr [esp + 40], 0
		jmp     TranscodeMultiByte

		#undef ReturnAddress
		#undef CodePage
		#undef dwFlags
		#undef lpMultiByteStr
		#undef cbMultiByte
		#undef lpUtf8Str
		#undef cbUtf8
	}
}
#endif

#undef Utf8ToMultiByte
#ifndef _M_IX86
int __stdcall Utf8ToMultiByte(
	UINT   CodePage,
	DWORD  dwFlags,
	LPCSTR lpUtf8Str,
	int    cbUtf8,
	LPSTR  lpMultiByteStr,
	int    cbMultiByte,
	LPCSTR lpDefaultChar,
	LPBOOL lpUsedDefaultChar)
{
	return
		TranscodeMultiByte(
			CP_UTF8,
			0,
			lpUtf8Str,
			cbUtf8,
			CodePage,
			dwFlags,
			lpMultiByteStr,
			cbMultiByte,
			lpDefaultChar,
			lpUsedDefaultChar);
}
#else
__declspec(naked) int __stdcall Utf8ToMultiByte(
	UINT   CodePage,
	DWORD  dwFlags,
	LPCSTR lpUtf8Str,
	int    cbUtf8,
	LPSTR  lpMultiByteStr,
	int    cbMultiByte,
	LPCSTR lpDefaultChar,
	LPBOOL lpUsedDefaultChar)
{
	__asm
	{
		#define ReturnAddress     esp
		#define CodePage          (esp + 4)
		#define dwFlags           (esp + 8)
		#define lpUtf8Str         (esp + 12)
		#define cbUtf8            (esp + 16)
		#define lpMultiByteStr    (esp + 20)
		#define cbMultiByte       (esp + 24)
		#define lpDefaultChar     (esp + 28)
		#define lpUsedDefaultChar (esp + 32)

		mov     eax, dword ptr [dwFlags]
		mov     ecx, dword ptr [cbUtf8]
		mov     dword ptr [esp + 8], ecx
		mov     dword ptr [esp + 16], eax
		mov     eax, dword ptr [ReturnAddress]
		mov     ecx, dword ptr [CodePage]
		mov     edx, dword ptr [lpUtf8Str]
		mov     dword ptr [esp], 0
		mov     dword ptr [esp + 4], edx
		mov     dword ptr [esp + 12], ecx
		push    CP_UTF8
		push    eax
		jmp     TranscodeMultiByte

		#undef ReturnAddress
		#undef CodePage
		#undef dwFlags
		#undef lpUtf8Str
		#undef cbUtf8
		#undef lpMultiByteStr
		#undef cbMultiByte
		#undef lpDefaultChar
		#undef lpUsedDefaultChar
	}
}
#endif
