#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

void __stdcall AddressNamingFromUtf8(unsigned long DataSize, char *tmpC, vector_string* tmpV);
void __stdcall AddressNamingFromUnicode(unsigned long DataSize, char *tmpC, vector_string* tmpV);
void __stdcall AddressNamingFEPNumber(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, vector *tmpV, unsigned long DataSize, char *tmpC);
void __stdcall AddressNamingFEPList(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, vector *tmpV, unsigned long DataSize, char *tmpC);
void __stdcall AddressNamingFEPFreeList(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, vector_string *tmpV, unsigned long DataSize, char *tmpC);

static void __stdcall AddressNamingFromANSI(unsigned long DataSize, char *tmpC, vector_string* tmpV) {
	extern HANDLE hHeap;
	LPSTR lpCharStr;
	string* format = &vector_type_at(tmpV, string, 5);
	if (string_empty(format)) return;
	if (lpCharStr = (LPSTR)HeapAlloc(hHeap, 0, DataSize + 1)) {
		__movsb((unsigned char *)lpCharStr, (const unsigned char *)tmpC, DataSize);
		lpCharStr[DataSize] = '\0';
		_snprintf(tmpC, DataSize + 1, string_c_str(format), lpCharStr);
		HeapFree(hHeap, 0, lpCharStr);
	} else {
		*tmpC = '\0';
	}
}

#define _BSWAP32(value) (            \
    (((value) >> 24) & 0x000000FF) | \
    (((value) >>  8) & 0x0000FF00) | \
    (((value) <<  8) & 0x00FF0000) | \
    (((value) << 24) & 0xFF000000))

__declspec(naked) void __cdecl AddressNamingAdditionalType()
{
	__asm
	{
		#define ReturnAddress 00506578H
		#define SSGCtrl       (edi)
		#define SSGS          (ebp + 16)
		#define tmpV          (esi)
		#define DataSize      (ebp - 01ECH)
		#define tmpC          (ebp - 01E8H)
		#define sizeof_string 24

		mov     eax, dword ptr [esi]
		mov     edx, dword ptr [tmpC]
		mov     ecx, dword ptr [eax + 4CH]
		mov     eax, dword ptr [eax + 48H]                  ;	const char *p = tmpV[3].c_str();
		sub     ecx, eax
		cmp     ecx, 4                                      ;	switch (tmpV[3].length())
		ja      L1                                          ;	{
		jb      L5                                          ;	case 4:
		cmp     dword ptr [eax], _BSWAP32('utf8')           ;		if (*(LPDWORD)p != BSWAP32('utf8'))
		jne     L5                                          ;			break;
		push    tmpV
		mov     eax, dword ptr [tmpV]                       ;		tmpV[3].clear();
		push    edx                                         ;		AddressNamingFromUtf8(DataSize, tmpC, tmpV);
		add     eax, sizeof_string * 3
		mov     edx, dword ptr [DataSize]
		mov     ecx, dword ptr [eax]
		push    edx
		mov     dword ptr [eax + 4], ecx
		mov     byte ptr [ecx], 0
		push    ReturnAddress                               ;
		jmp     AddressNamingFromUtf8                       ;		break;
	L1:
		sub     ecx, 7                                      ;	switch (tmpV[3].length())
		ja      L3                                          ;	{
		jb      L5                                          ;
		mov     ecx, dword ptr [eax]                        ;	case 7:
		mov     eax, dword ptr [eax + 4]                    ;
		cmp     ecx, _BSWAP32('unic')                       ;		if (*(LPDWORD)p == BSWAP32('unic'))
		jne     L2                                          ;		{
		cmp     eax, _BSWAP32('ode\0')                      ;			if (*(LPDWORD)(p + 4) != BSWAP32('ode\0'))
		jne     L5                                          ;				break;
		push    tmpV
		mov     eax, dword ptr [tmpV]                       ;			tmpV[3].clear();
		push    edx                                         ;			AddressNamingFromUnicode(DataSize, tmpC, tmpV);
		add     eax, sizeof_string * 3
		mov     edx, dword ptr [DataSize]
		mov     ecx, dword ptr [eax]
		push    edx
		mov     dword ptr [eax + 4], ecx
		mov     byte ptr [ecx], 0
		push    ReturnAddress                               ;			break;
		jmp     AddressNamingFromUnicode                    ;		}
	L2:
		xor     ecx, _BSWAP32('fep_')                       ;		if (*(LPDWORD)p != BSWAP32('fep_'))
		xor     eax, _BSWAP32('num\0')                      ;			break;
		or      eax, ecx                                    ;		if (*(LPDWORD)(p + 4) != BSWAP32('num\0'))
		jnz     L5                                          ;			break;
		mov     ecx, dword ptr [DataSize]                   ;		AddressNamingFEPNumber(SSGCtrl, SSGS, tmpV, DataSize, tmpC);
		mov     eax, dword ptr [SSGS]
		push    edx
		push    ecx
		push    tmpV
		push    eax
		push    SSGCtrl
		push    ReturnAddress
		jmp     AddressNamingFEPNumber                      ;		break;
	L3:
		cmp     dword ptr [eax], _BSWAP32('fep_')
		jne     L5
		dec     ecx                                         ;	case 8:
		jnz     L4                                          ;		if (*(LPDWORD)p != BSWAP32('fep_'))
		                                                    ;			break;
		cmp     dword ptr [eax + 4], _BSWAP32('list')       ;		if (*(LPDWORD)(p + 4) != BSWAP32('list'))
		jne     L5                                          ;			break;
		mov     eax, dword ptr [DataSize]                   ;		AddressNamingFEPList(SSGCtrl, SSGS, tmpV, DataSize, tmpC);
		mov     ecx, dword ptr [SSGS]
		push    edx
		push    eax
		push    tmpV
		push    ecx
		push    SSGCtrl
		push    ReturnAddress
		jmp     AddressNamingFEPList                        ;		break;
	L4:
		cmp     ecx, 13 - 8                                 ;	case 13:
		jne     L5                                          ;		if (*(LPDWORD)p != BSWAP32('fep_'))
		                                                    ;			break;
		cmp     byte ptr [eax + 12], 't'                    ;		if (*(LPDWORD)(p + 4) != BSWAP32('free'))
		jne     L5                                          ;			break;
		mov     ecx, dword ptr [eax + 4]                    ;		if (*(LPDWORD)(p + 8) != BSWAP32('_lis'))
		mov     eax, dword ptr [eax + 8]                    ;			break;
		xor     ecx, _BSWAP32('free')                       ;		if (p[12] != 't')
		xor     eax, _BSWAP32('_lis')                       ;			break;
		or      eax, ecx
		jnz     L5
		mov     eax, dword ptr [DataSize]                   ;		AddressNamingFEPFreeList(SSGCtrl, SSGS, tmpV, DataSize, tmpC);
		mov     ecx, dword ptr [SSGS]
		push    edx
		push    eax
		push    tmpV
		push    ecx
		push    SSGCtrl
		push    ReturnAddress
		jmp     AddressNamingFEPFreeList                    ;		break;
		                                                    ;	}
	L5:
		push    tmpV
		push    dword ptr [tmpC]
		push    dword ptr [DataSize]
		push    ReturnAddress
		jmp     AddressNamingFromANSI

		#undef ReturnAddress
		#undef SSGCtrl
		#undef SSGS
		#undef tmpV
		#undef DataSize
		#undef tmpC
		#undef sizeof_string
	}
}
