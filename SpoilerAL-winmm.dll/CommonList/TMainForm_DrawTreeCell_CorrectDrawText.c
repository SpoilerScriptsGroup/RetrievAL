#include <windows.h>
#include "bcb6_std_string.h"

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_CorrectDrawText()
{
	__asm
	{
		#define CallAddress 00415A60H

		mov     al, byte ptr [esp + 20]
		mov     ecx, CallAddress
		cmp     al, '+'
		je      L1
		jmp     ecx
		align   16
	L1:
		mov     ecx, dword ptr [esp + 12]
		mov     edx, dword ptr [ecx]
		align   16
	L2:
		mov     al, byte ptr [edx]
		inc     edx
		test    al, al
		jz      L4
		cmp     al, '='
		jne     L2
		align   16
	L3:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L3
		cmp     al, 0DH
		ja      L4
		cmp     al, 09H
		jae     L3
	L4:
		mov     eax, dword ptr [ecx + 4]
		sub     esp, 24
		dec     edx
		mov     ecx, esp
		push    eax
		call    bcb6_std_string_ctor_assign_range
		mov     ecx, dword ptr [esp + 24 + 4]
		mov     edx, dword ptr [esp + 24 + 8]
		mov     eax, CallAddress
		push    esp
		push    edx
		push    ecx
		call    eax
		add     esp, 12
		mov     ecx, esp
		call    bcb6_std_string_dtor
		add     esp, 24
		ret

		#undef CallAddress
	}
}
