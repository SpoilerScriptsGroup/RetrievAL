#include "bcb6_fastcall_bridge.h"

__declspec(naked) DWORD __fastcall bcb6_fastcall_bridge1(DWORD Function, DWORD A)
{
	__asm
	{
		mov     eax, edx
		jmp     ecx
	}
}

__declspec(naked) DWORD __fastcall _bcb6_fastcall_bridge2(DWORD Function, DWORD B, DWORD A)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, dword ptr [esp]
		mov     dword ptr [esp], eax
		mov     eax, dword ptr [esp + 4]
		mov     dword ptr [esp + 4], ecx
		ret
	}
}

__declspec(naked) DWORD __fastcall _bcb6_fastcall_bridge3(DWORD A, DWORD B, DWORD Function, DWORD C)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		mov     dword ptr [esp], ecx
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}
