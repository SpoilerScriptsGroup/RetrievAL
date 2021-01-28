#include <windows.h>

extern const DWORD _STL_string_M_assign_dispatch;

__declspec(naked) void __cdecl TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage()
{
	__asm
	{
		mov     eax, dword ptr [ebp - 1F4H]
		push    eax
		push    0044B312H
		jmp     dword ptr [_STL_string_M_assign_dispatch]
	}
}
