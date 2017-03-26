#include <windows.h>

__declspec(naked) void __cdecl TMainForm_SubjectAccess_CorrectFileName()
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
		cmp     cl, 0DH
		ja      L3
		cmp     cl, 09H
		jae     L2
	L3:
		dec     edx
		ret
	}
}
