#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_stack.h"

__declspec(naked) void __fastcall stack_dword_push(stack_dword *this, DWORD value)
{
	static const DWORD F004E4BC0 = 0x004E4BC0;

	__asm
	{
		push    ebx
		push    edx
		mov     eax, dword ptr [ecx + 24]
		mov     ebx, dword ptr [ecx + 16]
		sub     eax, 4
		cmp     eax, ebx
		je      L1
		mov     eax, dword ptr [ecx + 16]
		add     esp, 4
		add     eax, 4
		mov     dword ptr [ebx], edx
		mov     dword ptr [ecx + 16], eax
		pop     ebx
		ret

		align   16
	L1:
		push    esp
		push    ecx
		call    dword ptr [F004E4BC0]
		mov     ebx, dword ptr [esp + 12]
		add     esp, 16
		ret
	}
}

__declspec(naked) void __fastcall stack_dword_pop(stack_dword *this)
{
	static const DWORD F004B4DD0 = 0x004B4DD0;

	__asm
	{
		mov     eax, dword ptr [ecx + 16]
		mov     edx, dword ptr [ecx + 20]
		cmp     eax, edx
		je      L1
		sub     eax, 4
		mov     dword ptr [ecx + 16], eax
		ret

		align   16
	L1:
		push    ecx
		call    dword ptr [F004B4DD0]
		pop     ecx
		ret
	}
}
