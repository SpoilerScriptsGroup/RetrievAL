#include <windows.h>

__declspec(naked) void __cdecl TMainForm_SubjectAccess_StoreFileNamePrefix()
{
	__asm
	{
		mov     eax, dword ptr [esp + 12]
		mov     edx, dword ptr [esp + 16]
		mov     cl, byte ptr [eax]
		push    004EDFB8H
		cmp     cl, '+'
		je      L2
		cmp     cl, '*'
		jne     L1
		cmp     dword ptr [esp + 4], 005021CCH
		je      L2
	L1:
		mov     byte ptr [esp + 4 + 64], cl
		ret
		align   16
	L2:
		push    esi
		push    edi
		mov     edi, eax
		lea     esi, [eax + 1]
		mov     byte ptr [esp + 12 + 64], cl
		mov     ecx, edx
		dec     edx
		sub     ecx, eax
		rep movsb
		mov     dword ptr [esp + 12 + 16], edx
		pop     edi
		pop     esi
		ret
	}
}
