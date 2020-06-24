#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

extern const DWORD F00415A60;

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_CorrectDrawText()
{
	__asm
	{
		mov     al, byte ptr [esp + 20]
		mov     ecx, dword ptr [esp + 12]
		cmp     al, '+'
		je      L1
		jmp     dword ptr [F00415A60]

		align   16
	L1:
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
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L3
	L4:
		mov     eax, dword ptr [ecx + 4]
		sub     esp, 24
		dec     edx
		mov     ecx, esp
		push    eax
		call    string_ctor_assign_range
		mov     ecx, dword ptr [esp + 24 + 4]
		mov     edx, dword ptr [esp + 24 + 8]
		push    esp
		push    edx
		push    ecx
		call    dword ptr [F00415A60]
		add     esp, 12
		mov     ecx, esp
		call    string_dtor
		add     esp, 24
		ret
	}
}
