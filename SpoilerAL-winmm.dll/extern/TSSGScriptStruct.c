#define USING_NAMESPACE_BCB6_STD
#include "TSSGScriptStruct.h"

#pragma warning(disable:4733)

void(__cdecl * const TSSGScriptStruct_GetDistinction)(string *str, TSSGScriptStruct *SSGScriptStruct, const char *ID) = (LPVOID)0x00446158;

extern const DWORD F005D54CC;

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

__declspec(naked) long __stdcall TSSGScriptStruct_GetTitleTextWidth(TSSGScriptStruct *SSGScriptStruct, HDC hDC)
{
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
