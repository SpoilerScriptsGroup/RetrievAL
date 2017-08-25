#define USING_NAMESPACE_BCB6_STD
#define USING_NAMESPACE_BCB6_GLOBAL
#include "TIO_FEPAttribute.h"
#include "bcb6_global_operator.h"
#include "intrinsic.h"

#pragma warning(disable:4733)

extern const DWORD F005D54CC;

__declspec(naked) TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute()
{
	static const DWORD data1[] = {
		0x004EBA38,
		0x00002007, -4,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x00000000, -40,
		0x00050000, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000,
		(DWORD)data1,
	};

	__asm
	{
		push    ebp
		mov     eax, offset data2
		mov     ebp, esp
		sub     esp, 40
		call    dword ptr [F005D54CC]
		push    56
		call    dword ptr [operator_new]
		test    eax, eax
		jz      L1
		pop     ecx
		mov     dword ptr [ebp - 4], eax
		mov     dword ptr [eax + 4], 0
		inc     dword ptr [ebp - 12]
		mov     dword ptr [eax], 00640324H
		lea     ecx, [eax + 8]
		call    string_ctor
		mov     eax, dword ptr [ebp - 12]
		mov     ecx, dword ptr [ebp - 4]
		add     eax, 4
		add     ecx, 32
		mov     dword ptr [ebp - 12], eax
		call    string_ctor
		mov     eax, dword ptr [ebp - 4]
		mov     dword ptr [eax + 4], 32
	L1:
		mov     ecx, dword ptr [ebp - 40]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret
	}
}

__declspec(naked) void __fastcall delete_TIO_FEPAttribute(TIO_FEPAttribute *attribute)
{
	static const DWORD data1[] = {
		0x00000008, 0x00100400,
		0x004EBA38,
		0x00000002,
		BSWAP32('TIO_'),
		BSWAP32('FEPA'),
		BSWAP32('ttri'),
		BSWAP32('bute'),
		BSWAP32(' *[2'),
		BSWAP32(']\0\0\0'),
	};
	static const DWORD data2[] = {
		(DWORD)data1,
		0x0000500F, -8,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00000000, -44,
		0x00050000, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000,
		(DWORD)data2,
	};

	__asm
	{
		test    ecx, ecx
		jz      L1
		push    ebp
		mov     eax, offset data3
		mov     ebp, esp
		sub     esp, 44
		mov     dword ptr [ebp - 8], ecx
		call    dword ptr [F005D54CC]
		mov     eax, dword ptr [ebp - 8]
		push    3
		mov     ecx, dword ptr [eax]
		push    eax
		mov     dword ptr [ebp - 4], ecx
		call    dword ptr [ecx]
		mov     ecx, dword ptr [ebp - 44]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
	L1:
		ret
	}
}
