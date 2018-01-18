#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeSelector.h"

vector *(__cdecl * const TSSGAttributeSelector_MakeNowAttributeVec)(TSSGAttributeSelector *this) = (LPVOID)0x004D584C;
vector *(__cdecl * const TSSGAttributeSelector_AddElement)(TSSGAttributeSelector *this, LPVOID element) = (LPVOID)0x004D3DB8;
vector *(__cdecl * const TSSGAttributeSelector_PushElement)(TSSGAttributeSelector *this, LPVOID element) = (LPVOID)0x004D3CDC;
void(__cdecl * const TSSGAttributeSelector_PopElement)(TSSGAttributeSelector *this, LPVOID element) = (LPVOID)0x004D3E34;

__declspec(naked) void __cdecl TSSGAttributeSelector_PopElementByType(TSSGAttributeSelector *this, unsigned long type)
{
	__asm
	{
		push    ebp
		mov     eax, 004D3E46H
		mov     ebp, esp
		sub     esp, 84
		push    ebx
		push    esi
		push    edi
		mov     edi, dword ptr [ebp + 8]
		mov     edx, dword ptr [ebp + 12]
		jmp     eax
	}
}

__declspec(naked) void __cdecl TSSGAttributeSelector_EraseElementByType(TSSGAttributeSelector *this, unsigned long type)
{
	__asm
	{
		push    ebp
		mov     ecx, 004D40B2H
		mov     ebp, esp
		sub     esp, 64
		push    ebx
		push    esi
		push    edi
		lea     ebx, [ebp - 64]
		mov     eax, dword ptr [ebp + 12]
		jmp     ecx
	}
}
