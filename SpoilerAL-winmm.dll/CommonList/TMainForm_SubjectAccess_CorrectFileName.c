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
	L1:
		push    eax
		push    0
		mov     eax, esp
		push    0
		push    eax
		push    edx
		call    strtoul
		mov     edx, dword ptr [esp + 12]
		add     esp, 16
		mov     cl, byte ptr [edx]
		pop     eax
		test    cl, cl
		jz      L2
		inc     edx
	L2:
		ret
	}
}
