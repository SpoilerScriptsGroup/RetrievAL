#include <windows.h>

extern const DWORD F00415948;

__declspec(naked) void __cdecl TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage()
{
	__asm
	{
		mov     eax, dword ptr [ebp - 1F4H]
		push    eax
		push    0044B312H
		jmp     dword ptr [F00415948]
	}
}
