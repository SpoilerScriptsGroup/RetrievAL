#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <intrin.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

extern HANDLE hHeap;
#ifdef _NO_CRT_STDIO_INLINE
void __fastcall AddressNamingFromUnicode(
	unsigned long  const DataSize,
	char          *const tmpC,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGCtrl,
	TSSGSubject   *const SSGS)
{

	LPWSTR lpWideCharStr;
	string *const format = &vector_at(tmpV, 5);
	if (lpWideCharStr = HeapAlloc(hHeap, 0, DataSize + sizeof(wchar_t)))
	{
		__movsw(lpWideCharStr, (wchar_t *)tmpC, DataSize >> 1);
		lpWideCharStr[DataSize >> 1] = L'\0';
		_snprintf(tmpC, DataSize + 1, string_empty(format) ? "%ls" : string_c_str(format), lpWideCharStr);
		HeapFree(hHeap, 0, lpWideCharStr);
	}
	else
	{
		*tmpC = '\0';
		SetLastError(ERROR_OUTOFMEMORY);
	}
	string_clear(format);
}
#else
__declspec(naked) void __fastcall AddressNamingFromUnicode(
	unsigned long  const DataSize,
	char          *const tmpC,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGCtrl,
	TSSGSubject   *const SSGS)
{
	__asm
	{
		#define DataSize ecx
		#define tmpC     edx

		push    esi
		push    edi
		mov     eax, dword ptr [hHeap]
		mov     edi, DataSize                               ; DWORD dwBytes = DataSize + 2;
		mov     esi, tmpC
		xor     ecx, ecx
		push    ecx
		push    ecx
		push    eax
		push    ecx
		inc     edi
		push    ecx
		push    edi
		push    esi
		push    -1
		push    ecx
		push    ecx
		inc     edi
		push    CP_THREAD_ACP
		push    edi                                         ; LPWSTR lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, dwBytes);
		push    ecx
		push    eax
		call    HeapAlloc
		test    eax, eax                                    ; if (lpWideCharStr != NULL)
		jz      L1                                          ; {
		mov     ecx, edi                                    ;     DWORD dwCount = (dwBytes >> 1) - 1;
		mov     edi, eax                                    ;     __movsw((unsigned short *)lpWideCharStr, (const unsigned short *)tmpC, dwCount);
		shr     ecx, 1
		mov     dword ptr [esp + 8], eax
		dec     ecx
		mov     dword ptr [esp + 40], eax
		rep movsw
		mov     word ptr [edi], cx                          ;     lpWideCharStr[dwCount] = L'\0';
		call    WideCharToMultiByte                         ;     WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, -1, tmpC, DataSize + 1, NULL, NULL);
		call    HeapFree                                    ;     HeapFree(hHeap, 0, lpWideCharStr);
		jmp     L2                                          ; }
		                                                    ; else
		                                                    ; {
	L1:
		add     esp, 44
		mov     byte ptr [esi], al                          ;     *tmpC = '\0';
		                                                    ; }
	L2:
		pop     edi
		pop     esi
		ret     12

		#undef DataSize
		#undef tmpC
	}
}
#endif
