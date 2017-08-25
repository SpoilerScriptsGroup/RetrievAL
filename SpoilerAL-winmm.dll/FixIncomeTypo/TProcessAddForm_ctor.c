#include <windows.h>

extern const DWORD F00401FD8;
extern const DWORD _TWinControl_GetHandle;

__declspec(naked) void __cdecl TProcessAddForm_ctor()
{
	static const char data1[] = "â“ì≠ÉvÉçÉZÉXÇÃçƒéÊìæ";

	__asm
	{
		inc     dword ptr [ebx + 1CH]
		inc     dword ptr [ebx + 1CH]
		push    offset data1
		mov     eax, dword ptr [ebp - 4H]
		mov     eax, dword ptr [eax + 30CH]
		call    dword ptr [_TWinControl_GetHandle]
		push    eax
		push    00486C8FH
		jmp     SetWindowTextA
	}
}
