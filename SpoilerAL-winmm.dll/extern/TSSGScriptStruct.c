#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSGScriptStruct.h"
#include "bcb6_operator.h"

#pragma warning(disable:4733)

string *(__cdecl * const TSSGScriptStruct_GetDistinction)(string *Result, const TSSGScriptStruct *this, const char *ID) = (LPVOID)0x00446158;

__declspec(naked) void __cdecl TSSGScriptStruct_SetDistinction(const TSSGScriptStruct *this, const char *ID, const char *Val)
{
	extern const DWORD F00402590;
	extern const DWORD F00415878;
	extern const DWORD F004159D4;
	extern const DWORD F004165E8;
	extern const DWORD F004166F0;
	extern const DWORD F004167F8;
	extern const DWORD F005D5258;
	extern const DWORD F005D534C;
	extern const DWORD F005D54CC;

	static const DWORD data1[] = {
		0x00402634,
		0x00000004, -8,
		0x004024F8,
		0x00000004, -32,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x004024F8,
		0x00000004, -56,
		0x00416304,
		0x00000004, -104,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00000000, -140,
		0x00050000, 0x00000000,
		(DWORD)data1,
		0x00050008, 0x00000005,
		(DWORD)data2,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
	};

	__asm
	{
		push    ebp
		mov     ebp, esp
		sub     esp, 232
		mov     eax, offset data3
		push    ebx
		push    esi
		push    edi
		mov     esi, dword ptr [ebp + 8H]
		call    dword ptr [F005D54CC]
		mov     ebx, dword ptr [ebp + 10H]
		lea     eax, [ebp - 8H]
		mov     word ptr [ebp - 7CH], 8
		push    eax
		lea     ecx, [ebp - 20H]
		inc     dword ptr [ebp - 70H]
		mov     edx, dword ptr [ebp + 0CH]
		push    edx
		push    ecx
		call    dword ptr [F004165E8]
		add     esp, 12
		lea     eax, [ebp - 20H]
		add     dword ptr [ebp - 70H], 4
		push    eax
		push    esi
		call    dword ptr [F00415878]
		add     esp, 8
		mov     dword ptr [ebp - 90H], eax
		mov     eax, dword ptr [esi + 8H]
		mov     edx, eax
		mov     dword ptr [ebp - 94H], edx
		cmp     edx, dword ptr [ebp - 90H]
		sete    cl
		and     ecx, 01H
		test    cl, cl
		jne     L7
		mov     edi, dword ptr [ebp - 90H]
		mov     eax, dword ptr [esi + 18H]
		add     edi, 16
		mov     dword ptr [ebp - 9CH], eax
		mov     eax, dword ptr [esi + 1CH]
		mov     dword ptr [ebp - 98H], eax
		mov     edx, dword ptr [edi + 4H]
		mov     dword ptr [ebp - 0A0H], edx
		mov     ecx, dword ptr [edi]
		mov     dword ptr [ebp - 0A4H], ecx
		mov     eax, dword ptr [ebp - 1CH]
		mov     dword ptr [ebp - 0A8H], eax
		mov     edx, dword ptr [ebp - 20H]
		mov     dword ptr [ebp - 0ACH], edx
		mov     ecx, dword ptr [ebp - 0A8H]
		sub     ecx, dword ptr [ebp - 0ACH]
		mov     dword ptr [ebp - 0B0H], ecx
		mov     eax, dword ptr [ebp - 0A0H]
		sub     eax, dword ptr [ebp - 0A4H]
		mov     dword ptr [ebp - 0B4H], eax
		mov     edx, dword ptr [ebp - 0B4H]
		cmp     edx, dword ptr [ebp - 0B0H]
		jge     L1
		lea     ecx, [ebp - 0B4H]
		jmp     L2
	L1:
		lea     ecx, [ebp - 0B0H]
	L2:
		mov     eax, dword ptr [ecx]
		mov     dword ptr [ebp - 0BCH], eax
		mov     edx, dword ptr [ebp - 0BCH]
		push    edx
		mov     eax, dword ptr [ebp - 0A4H]
		push    eax
		mov     edx, dword ptr [ebp - 0ACH]
		push    edx
		call    dword ptr [F005D534C]
		add     esp, 12
		mov     dword ptr [ebp - 0B8H], eax
		cmp     dword ptr [ebp - 0B8H], 0
		jz      L3
		mov     ecx, dword ptr [ebp - 0B8H]
		jmp     L6
	L3:
		mov     eax, dword ptr [ebp - 0B0H]
		cmp     eax, dword ptr [ebp - 0B4H]
		jge     L4
		or      ecx, 0FFFFFFFFH
		jmp     L6
	L4:
		mov     edx, dword ptr [ebp - 0B0H]
		cmp     edx, dword ptr [ebp - 0B4H]
		jle     L5
		mov     ecx, 1
		jmp     L6
	L5:
		xor     ecx, ecx
	L6:
		test    ecx, ecx
		setl    al
		and     eax, 01H
		test    al, al
		je      L8
	L7:
		mov     word ptr [ebp - 7CH], 20
		lea     edx, [ebp - 38H]
		push    edx
		call    dword ptr [F00402590]
		pop     ecx
		add     dword ptr [ebp - 70H], 4
		mov     dword ptr [ebp - 0C4H], eax
		lea     eax, [ebp - 20H]
		push    eax
		lea     edx, [ebp - 68H]
		push    edx
		call    dword ptr [F004166F0]
		add     esp, 8
		add     dword ptr [ebp - 70H], 4
		mov     ecx, dword ptr [ebp - 0C4H]
		push    ecx
		lea     eax, [ebp - 50H]
		push    eax
		call    dword ptr [F004166F0]
		add     dword ptr [ebp - 70H], 4
		inc     dword ptr [ebp - 70H]
		lea     edx, [ebp - 68H]
		mov     dword ptr [ebp - 0C0H], edx
		add     esp, 8
		mov     ecx, dword ptr [ebp - 90H]
		mov     dword ptr [ebp - 0C8H], ecx
		lea     ecx, [ebp - 0CCH]
		mov     eax, dword ptr [ebp - 0C0H]
		push    eax
		sub     esp, 4
		mov     edx, dword ptr [ebp - 0C8H]
		mov     dword ptr [esp], edx
		push    esi
		push    ecx
		call    dword ptr [F004167F8]
		add     esp, 16
		lea     eax, [ebp - 0CCH]
		mov     edx, dword ptr [eax]
		mov     dword ptr [ebp - 90H], edx
		sub     dword ptr [ebp - 70H], 9
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		mov     ecx, dword ptr [ebp - 40H]
		sub     ecx, dword ptr [ebp - 50H]
		mov     dword ptr [ebp - 0D0H], ecx
		mov     edi, dword ptr [ebp - 50H]
#if !OPTIMIZE_ALLOCATOR
		mov     edx, dword ptr [ebp - 0D0H]
#endif
		mov     ecx, edi
		call    internal_deallocate
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		mov     edx, dword ptr [ebp - 58H]
		sub     edx, dword ptr [ebp - 68H]
		mov     dword ptr [ebp - 0D4H], edx
		mov     ecx, dword ptr [ebp - 68H]
		mov     dword ptr [ebp - 0D8H], ecx
#if !OPTIMIZE_ALLOCATOR
		mov     edx, dword ptr [ebp - 0D4H]
#endif
		mov     ecx, dword ptr [ebp - 0D8H]
		call    internal_deallocate
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		mov     eax, dword ptr [ebp - 28H]
		sub     eax, dword ptr [ebp - 38H]
		mov     dword ptr [ebp - 0DCH], eax
		mov     edx, dword ptr [ebp - 38H]
		mov     dword ptr [ebp - 0E0H], edx
#if !OPTIMIZE_ALLOCATOR
		mov     edx, dword ptr [ebp - 0DCH]
#endif
		mov     ecx, dword ptr [ebp - 0E0H]
		call    internal_deallocate
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		mov     word ptr [ebp - 7CH], 8
	L8:
		push    ebx
		call    dword ptr [F005D5258]
		pop     ecx
		add     eax, ebx
		lea     ecx, [ebp - 0E8H]
		push    ecx
		push    eax
		push    ebx
		mov     eax, dword ptr [ebp - 90H]
		add     eax, 40
		push    eax
		call    dword ptr [F004159D4]
		add     esp, 16
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		mov     ecx, dword ptr [ebp - 20H]
#if !OPTIMIZE_ALLOCATOR
		mov     edx, dword ptr [ebp - 10H]
		sub     edx, ecx
#endif
		call    internal_deallocate
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		dec     dword ptr [ebp - 70H]
		mov     eax, dword ptr [ebp - 8CH]
		mov     dword ptr fs:[0], eax
		pop     edi
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret
	}
}

static const DWORD data1[] = {
	0x00416274,
	0x00000004, -24,
	0x00000000,
};
const DWORD TSSGScriptStruct_GetTitleTextWidth_CtorData[] = {
	0x00000000, -60,
	0x00050000, 0x00000000,
	(DWORD)data1,
	0x00050000, 0x00000000,
	(DWORD)data1,
	0x00050014, 0x00000000,
	0x00000000, 0x00050014,
	0x00000000, 0x00000000,
};

__declspec(naked) long __stdcall TSSGScriptStruct_GetTitleTextWidth(const TSSGScriptStruct *this, HDC hDC)
{
	extern const DWORD F005D54CC;

	__asm
	{
		#define _lpszTitle 006030FFH

		push    ebp
		mov     eax, offset TSSGScriptStruct_GetTitleTextWidth_CtorData
		mov     ebp, esp
		sub     esp, 68
		call    dword ptr [F005D54CC]
		mov     edx, dword ptr [ebp + 8H]
		lea     ecx, [ebp - 18H]
		push    _lpszTitle
		push    edx
		push    ecx
		call    dword ptr [TSSGScriptStruct_GetDistinction]
		add     esp, 12
		lea     eax, [ebp - 44H]
		mov     ecx, dword ptr [ebp - 18H]
		mov     edx, dword ptr [ebp - 14H]
		sub     edx, ecx
		push    eax
		mov     eax, dword ptr [ebp + 0CH]
		push    edx
		push    ecx
		push    eax
		call    GetTextExtentPoint32A
		lea     ecx, [ebp - 18H]
		call    string_dtor
		mov     ecx, dword ptr [ebp - 60]
		mov     eax, dword ptr [ebp - 44H]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret     8

		#undef _lpszTitle
	}
}
