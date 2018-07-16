#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <windows.h>
#include <intrin.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

extern HANDLE hHeap;

#ifdef _NO_CRT_STDIO_INLINE
void __stdcall AddressNamingFromUnicode(unsigned long DataSize, char *tmpC, vector_string* tmpV)
{
	LPWSTR lpWideCharStr;
	string* format = &vector_type_at(tmpV, string, 5);
	if (lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, DataSize + sizeof(wchar_t))) {
		__movsw((unsigned short *)lpWideCharStr, (const unsigned short *)tmpC, DataSize >> 1);
		lpWideCharStr[DataSize >> 1] = L'\0';
		_snprintf(tmpC, DataSize + 1, string_empty(format) ? "%S" : string_c_str(format), lpWideCharStr);
		HeapFree(hHeap, 0, lpWideCharStr);
	} else {
		*tmpC = '\0';
	}
}
#else
__declspec(naked) void __stdcall AddressNamingFromUnicode(unsigned long DataSize, char *tmpC, vector_string* tmpV)
{
	__asm
	{
		#define DataSize (esp + 4)
		#define tmpC     (esp + 8)

		push    esi
		push    edi
		mov     eax, dword ptr [hHeap]
		xor     ecx, ecx
		mov     edi, dword ptr [DataSize + 8]               ; DWORD dwBytes = DataSize + 2;
		mov     esi, dword ptr [tmpC + 8]
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
