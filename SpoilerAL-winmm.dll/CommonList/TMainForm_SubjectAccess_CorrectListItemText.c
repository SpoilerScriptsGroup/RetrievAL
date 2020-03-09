#include <windows.h>

__declspec(naked) void __cdecl TMainForm_SubjectAccess_CorrectListItemText()
{
	__asm
	{
		mov     cl, byte ptr [esp + 4]
		push    005E0D50H
		cmp     cl, '+'
		je      L1
		ret

		align   16
	L1:
		mov     cl, byte ptr [edx]
		inc     edx
		test    cl, cl
		jz      L3
		cmp     cl, '='
		jne     L1

		align   16
	L2:
		mov     cl, byte ptr [edx]
		inc     edx
		cmp     cl, ' '
		je      L2
		sub     cl, '\t'
		cmp     cl, '\r' - '\t' + 1
		jb      L2
	L3:
		dec     edx
		ret
	}
}
