#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#pragma function(memcpy)

#pragma warning(disable:4733)

void __cdecl TSSGCtrl_TrimString();

extern const DWORD F00439D1C;
extern const DWORD F0050123C;
extern const DWORD F005D54CC;

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSR_SwitchTmpS_0()
{
	static const DWORD data1[] = {
		0x00402634,
		0x00000004, -8,
		0x00402634,
		0x00000004, -16,
		0x00416274,
		0x00000004, -40,
		0x00416274,
		0x00000004, -64,
		0x00416274,
		0x00000004, -88,
		0x00501888,
		0x00000004, -120,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x00402634,
		0x00000004, -8,
		0x00416274,
		0x00000004, -328,
		0x00402634,
		0x00000004, -16,
		0x00416274,
		0x00000004, -40,
		0x00416274,
		0x00000004, -64,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00402634,
		0x00000004, -16,
		0x00416274,
		0x00000004, -368,
		0x00000000,
	};
	static const DWORD data4[] = {
		0x00501888,
		0x00000004, -152,
		0x00000000,
	};
	static const DWORD data5[] = {
		0x00000000, -188,
		0x00050000, 0x00000000,
		(DWORD)data1,
		0x00050008, 0x00000000,
		(DWORD)data2,
		0x00050014, 0x00000005,
		(DWORD)data3,
		0x00050008, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000,
		(DWORD)data4,
		0x00050098, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000,
	};

	__asm
	{
		sub     al, '@' - '#'
		je      L1
		cmp     al, 'F' - '@'
		je      FormatPrefix
		mov     ecx, 004FFDC5H
		jmp     ecx
	L1:
		mov     ecx, 005009A5H
		jmp     ecx

	FormatPrefix:

		#define _this                  (ebp + 5A8H + 8H)
		#define offsetof_TSSGCtrl_strD 992
		#define LineList               (ebp + 5A8H - 40H)
		#define tmpS                   (ebp + 5A8H - 18H)

		push    ebp
		mov     eax, offset data5
		mov     ebp, esp
		sub     esp, 284
		push    ebx
		push    esi
		mov     ebx, dword ptr [_this]
		lea     esi, [tmpS]
		add     ebx, offsetof_TSSGCtrl_strD
		call    dword ptr [F005D54CC]
		push    0
		push    0
		sub     esp, 24
		mov     edx, 00631BFDH
		mov     ecx, esp
		call    string_ctor_assign_cstr
		push    -1
		push    1
		push    0
		push    0
		sub     esp, 24
		mov     edx, 00631BFBH
		mov     ecx, esp
		call    string_ctor_assign_cstr
		push    esi
		lea     ecx, [ebp - 28H]
		push    ebx
		push    ecx
		call    TStringDivision_Half
		add     esp, 44
		lea     edx, [ebp - 28H]
		push    edx
		lea     ecx, [ebp - 40H]
		push    ecx
		call    dword ptr [F00439D1C]
		add     esp, 16
		lea     ecx, [ebp - 40H]
		lea     eax, [ebp - 58H]
		push    ecx
		push    ebx
		push    eax
		call    TSSGCtrl_TrimString
		add     esp, 44
		lea     edx, [ebp - 58H]
		mov     byte ptr [ebp - 0C5H], 32
		lea     ecx, [ebp - 70H]
		mov     dword ptr [ebp - 0C4H], edx
		mov     byte ptr [ebp - 78H], 32
		call    string_ctor_assign
		lea     eax, [LineList]
		lea     edx, [ebp - 78H]
		mov     eax, dword ptr [eax + 8H]
		mov     dword ptr [ebp - 0C0H], edx
		mov     dword ptr [ebp - 0CCH], eax
		lea     ecx, [LineList]
		push    edx
		push    ecx
		call    dword ptr [F0050123C]
		mov     edx, dword ptr [ebp - 0CCH]
		add     esp, 8
		mov     dword ptr [ebp - 0D0H], edx
		mov     ecx, dword ptr [edx + 4H]
		mov     dword ptr [ebp - 0D4H], ecx
		mov     dword ptr [eax], edx
		mov     dword ptr [eax + 4H], ecx
		mov     dword ptr [ecx], eax
		mov     dword ptr [edx + 4H], eax
		mov     dword ptr [ebp - 0D8H], eax
		lea     ecx, [ebp - 70H]
		call    string_dtor
		lea     ecx, [ebp - 58H]
		call    string_dtor
		lea     ecx, [ebp - 40H]
		call    string_dtor
		lea     ecx, [ebp - 28H]
		call    string_dtor
		mov     eax, dword ptr [esi]
		jmp     L3
	L2:
		inc     eax
	L3:
		mov     dl, byte ptr [eax]
		cmp     dl, 32
		je      L2
		cmp     dl, 9
		jb      L4
		cmp     dl, 13
		jbe     L2
	L4:
		mov     edx, dword ptr [esi + 4H]
		dec     edx
		jmp     L6
	L5:
		dec     edx
	L6:
		cmp     eax, edx
		jae     L7
		mov     cl, byte ptr [edx]
		cmp     cl, 32
		je      L5
		cmp     cl, 9
		jb      L7
		cmp     cl, 13
		jbe     L5
	L7:
		inc     edx
		mov     ecx, dword ptr [esi]
		cmp     edx, dword ptr [esi + 4H]
		jne     L8
		cmp     eax, ecx
		je      L11
	L8:
		sub     edx, eax
		mov     dword ptr [ebp - 0F0H], edx
		cmp     eax, ecx
		je      L9
		push    edx
		push    eax
		push    ecx
		call    memcpy
		add     esp, 12
	L9:
		mov     eax, dword ptr [esi + 4H]
		mov     ecx, dword ptr [esi]
		sub     eax, ecx
		mov     dword ptr [ebp - 0F4H], edx
		cmp     eax, edx
		jb      L10
		mov     eax, dword ptr [esi + 4H]
		add     ecx, edx
		mov     dword ptr [ebp - 0F8H], eax
		mov     dword ptr [ebp - 0FCH], ecx
		cmp     ecx, eax
		je      L11
		mov     dword ptr [ebp - 100H], 1
		push    1
		push    eax
		push    ecx
		call    memmove
		mov     eax, dword ptr [esi + 4H]
		mov     ecx, dword ptr [ebp - 0F8H]
		sub     eax, ecx
		mov     ecx, dword ptr [ebp - 0FCH]
		add     eax, ecx
		add     esp, 12
		mov     dword ptr [ebp - 104H], eax
		mov     dword ptr [esi + 4H], eax
		jmp     L11
	L10:
		mov     eax, dword ptr [esi + 4H]
		add     edx, ecx
		sub     edx, eax
		push    0
		mov     ecx, esi
		call    string_append
	L11:
		mov     eax, 00641AE4H
		mov     edx, esi
		mov     al, byte ptr [eax]
		lea     ecx, [ebp - 90H]
		mov     byte ptr [ebp - 98H], al
		call    string_ctor_assign
		lea     eax, [LineList]
		lea     edx, [ebp - 98H]
		mov     eax, dword ptr [eax + 8H]
		lea     ecx, [LineList]
		mov     dword ptr [ebp - 108H], edx
		mov     dword ptr [ebp - 10CH], eax
		mov     dword ptr [ebp - 110H], eax
		push    edx
		push    ecx
		call    dword ptr [F0050123C]
		mov     edx, dword ptr [ebp - 110H]
		add     esp, 8
		mov     ecx, dword ptr [edx + 4H]
		mov     dword ptr [ebp - 114H], ecx
		mov     dword ptr [eax], edx
		mov     dword ptr [eax + 4H], ecx
		mov     dword ptr [ecx], eax
		mov     dword ptr [edx + 4H], eax
		mov     dword ptr [ebp - 118H], eax
		lea     ecx, [ebp - 90H]
		call    string_dtor
		mov     ecx, dword ptr [ebp - 188]
		mov     dword ptr fs:[0], ecx
		pop     esi
		pop     ebx
		mov     esp, ebp
		mov     eax, 00500DFFH
		pop     ebp
		jmp     eax

		#undef _this
		#undef offsetof_TSSGCtrl_strD
		#undef LineList
		#undef tmpS
	}
}
