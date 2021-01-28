#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

void __fastcall AddressNamingFromUtf8   (unsigned long DataSize, char *tmpC, vector_string *tmpV, TSSGCtrl *SSGCtrl, TSSGSubject *SSGS);
void __fastcall AddressNamingFromUnicode(unsigned long DataSize, char *tmpC, vector_string *tmpV, TSSGCtrl *SSGCtrl, TSSGSubject *SSGS);
void __fastcall AddressNamingFEPNumber  (unsigned long DataSize, char *tmpC, vector_string *tmpV, TSSGCtrl *SSGCtrl, TSSGSubject *SSGS);
void __fastcall AddressNamingFEPList    (unsigned long DataSize, char *tmpC, vector_string *tmpV, TSSGCtrl *SSGCtrl, TSSGSubject *SSGS);
void __fastcall AddressNamingFEPFreeList(unsigned long DataSize, char *tmpC, vector_string *tmpV, TSSGCtrl *SSGCtrl, TSSGSubject *SSGS);

__declspec(naked) void __stdcall AddressNamingAdditionalType(vector_string *tmpV, TSSGCtrl *SSGCtrl, TSSGSubject *SSGS)
{
	__asm
	{
		#define SSGCtrl  (edi)
		#define SSGS     (ebp + 16)
		#define tmpV     (esi)
		#define DataSize (ebp - 01ECH)
		#define tmpC     (ebp - 01E8H)

		mov     edx, dword ptr [tmpC]
		mov     eax, dword ptr [tmpV]
		mov     ecx, [eax + size string * 3]string._M_finish
		mov     eax, [eax + size string * 3]string._M_start ;	const char *p = tmpV[3].c_str();
		sub     ecx, eax
		cmp     ecx, 4                                      ;	switch (tmpV[3].length())
		ja      L1                                          ;	{
		jb      L5                                          ;	case 4:
		cmp     dword ptr [eax], MASM_BSWAP32('utf8')       ;		if (*(LPDWORD)p != BSWAP32('utf8'))
		jne     L5                                          ;			break;
		mov     ecx, dword ptr [tmpV]                       ;		tmpV[3].clear();
		mov     eax, [ecx + size string * 3]string._M_start ;		AddressNamingFromUtf8(DataSize, tmpC, tmpV, SSGCtrl, SSGS);
		mov     byte ptr [eax], 0
		mov     [ecx + size string * 3]string._M_finish, eax
		mov     ecx, dword ptr [DataSize]
		jmp     AddressNamingFromUtf8                       ;		return;

	L1:
		sub     ecx, 7                                      ;
		ja      L3                                          ;
		jb      L5                                          ;
		mov     ecx, dword ptr [eax]                        ;	case 7:
		mov     eax, dword ptr [eax + 4]                    ;
		cmp     ecx, MASM_BSWAP32('unic')                   ;		if (*(LPDWORD)p == BSWAP32('unic'))
		jne     L2                                          ;		{
		cmp     eax, MASM_BSWAP32('ode\0')                  ;			if (*(LPDWORD)(p + 4) != BSWAP32('ode\0'))
		jne     L5                                          ;				break;
		mov     ecx, dword ptr [tmpV]                       ;			tmpV[3].clear();
		mov     eax, [ecx + size string * 3]string._M_start ;			AddressNamingFromUnicode(DataSize, tmpC, tmpV, SSGCtrl, SSGS);
		mov     byte ptr [eax], 0
		mov     [ecx + size string * 3]string._M_finish, eax;			return;
		mov     ecx, dword ptr [DataSize]
		jmp     AddressNamingFromUnicode                    ;		}

	L2:
		xor     ecx, MASM_BSWAP32('fep_')                   ;		if (*(LPDWORD)p != BSWAP32('fep_'))
		xor     eax, MASM_BSWAP32('num\0')                  ;			break;
		or      eax, ecx                                    ;		if (*(LPDWORD)(p + 4) != BSWAP32('num\0'))
		jnz     L5                                          ;			break;
		mov     ecx, dword ptr [DataSize]                   ;		AddressNamingFEPNumber(DataSize, tmpC, tmpV, SSGCtrl, SSGS);
		jmp     AddressNamingFEPNumber                      ;		return;

	L3:
		cmp     dword ptr [eax], MASM_BSWAP32('fep_')
		jne     L5
		dec     ecx                                         ;	case 8:
		jnz     L4                                          ;		if (*(LPDWORD)p != BSWAP32('fep_'))
		                                                    ;			break;
		cmp     dword ptr [eax + 4], MASM_BSWAP32('list')   ;		if (*(LPDWORD)(p + 4) != BSWAP32('list'))
		jne     L5                                          ;			break;
		mov     ecx, dword ptr [DataSize]                   ;		AddressNamingFEPList(DataSize, tmpC, tmpV, SSGCtrl, SSGS);
		jmp     AddressNamingFEPList                        ;		return;

	L4:
		cmp     ecx, 13 - 8                                 ;	case 13:
		jne     L5                                          ;		if (*(LPDWORD)p != BSWAP32('fep_'))
		                                                    ;			break;
		cmp     byte ptr [eax + 12], 't'                    ;		if (*(LPDWORD)(p + 4) != BSWAP32('free'))
		jne     L5                                          ;			break;
		mov     ecx, dword ptr [eax + 4]                    ;		if (*(LPDWORD)(p + 8) != BSWAP32('_lis'))
		mov     eax, dword ptr [eax + 8]                    ;			break;
		xor     ecx, MASM_BSWAP32('free')                   ;		if (p[12] != 't')
		xor     eax, MASM_BSWAP32('_lis')                   ;			break;
		or      eax, ecx
		jnz     L5
		mov     ecx, dword ptr [DataSize]                   ;		AddressNamingFEPFreeList(DataSize, tmpC, tmpV, SSGCtrl, SSGS);
		jmp     AddressNamingFEPFreeList                    ;		return;
		                                                    ;	}
	L5:
		ret     size PVOID * 3

		#undef SSGCtrl
		#undef SSGS
		#undef tmpV
		#undef DataSize
		#undef tmpC
	}
}
