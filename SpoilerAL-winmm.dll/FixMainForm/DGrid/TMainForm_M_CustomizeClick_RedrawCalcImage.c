#include <windows.h>

extern const DWORD _TMainForm_SubjectAccess;
extern const DWORD F0048292C;
extern const DWORD F00483678;
extern const DWORD F004836B4;

__declspec(naked) void __fastcall TMainForm_M_CustomizeClick_RedrawCalcImage(void *this)
{
	__asm
	{
		push    ebx
		push    esi
		mov     ebx, ecx
		mov     esi, dword ptr [ecx + 528H]
		push    18
		push    esi
		call    dword ptr [F004836B4]
		mov     cl, byte ptr [ebx + 4D0H]
		mov     dword ptr [esp], 18
		test    cl, cl
		jnz     L1
		inc     al
	L1:
		mov     dword ptr [esp + 4], eax
		push    esi
		call    dword ptr [F00483678]
		add     esp, 12
		xor     ecx, ecx
		mov     edx, 18
		mov     eax, esi
		call    dword ptr [F0048292C]
		mov     eax, dword ptr [ebx + 55CH]
		cmp     eax, 1
		je      L2
		cmp     eax, 6
		jne     L3
	L2:
		mov     edx, dword ptr [ebx + 524H]
		push    edx
		push    ebx
		call    dword ptr [_TMainForm_SubjectAccess]
		add     esp, 8
	L3:
		pop     esi
		pop     ebx
		ret
	}
}
