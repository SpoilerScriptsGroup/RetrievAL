#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeSelector.h"

void(__cdecl * const TSSGAttributeSelector_StartElementCheck)(TSSGAttributeSelector *this) = (LPVOID)0x004D2D54;
void(__cdecl * const TSSGAttributeSelector_EndElementCheck)(TSSGAttributeSelector *this) = (LPVOID)0x004D3670;
vector *(__cdecl * const TSSGAttributeSelector_AddElement)(TSSGAttributeSelector *this, LPVOID element) = (LPVOID)0x004D3DB8;
vector *(__cdecl * const TSSGAttributeSelector_PushElement)(TSSGAttributeSelector *this, LPVOID element) = (LPVOID)0x004D3CDC;
void(__cdecl * const TSSGAttributeSelector_PopElement)(TSSGAttributeSelector *this, LPVOID element) = (LPVOID)0x004D3E34;
void(__cdecl * const TSSGAttributeSelector_ToRootElement)(TSSGAttributeSelector *this, LPVOID element) = (LPVOID)0x004D41FC;
vector *(__cdecl * const TSSGAttributeSelector_MakeNowAttributeVec)(TSSGAttributeSelector *this) = (LPVOID)0x004D584C;

__declspec(naked) void __cdecl TSSGAttributeSelector_PopElementByType(TSSGAttributeSelector *this, unsigned long type)
{
	static const DWORD X004D3E46 = 0x004D3E46;

	__asm
	{
		mov     edx, dword ptr [esp + 8]
		push    ebp
		mov     ebp, esp
		sub     esp, 84
		push    ebx
		push    esi
		push    edi
		mov     edi, dword ptr [ebp + 8]
		jmp     dword ptr [X004D3E46]
	}
}

__declspec(naked) void __cdecl TSSGAttributeSelector_EraseElementByType(TSSGAttributeSelector *this, unsigned long type)
{
	static const DWORD X004D40B2 = 0x004D40B2;

	__asm
	{
		mov     eax, dword ptr [esp + 8]
		push    ebp
		mov     ebp, esp
		sub     esp, 64
		push    ebx
		push    esi
		push    edi
		lea     ebx, [ebp - 64]
		jmp     dword ptr [X004D40B2]
	}
}

__declspec(naked) void __cdecl TSSGAttributeSelector_ToRootElementByType(TSSGAttributeSelector *this, unsigned long type)
{
	static const DWORD X004D420E = 0x004D420E;

	__asm
	{
		mov     edx, dword ptr [esp + 8]
		push    ebp
		mov     ebp, esp
		sub     esp, 64
		push    ebx
		push    esi
		push    edi
		mov     esi, dword ptr [ebp + 8]
		jmp     dword ptr [X004D420E]
	}
}
