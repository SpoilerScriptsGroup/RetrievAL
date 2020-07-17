#include "TWinControl.h"

__declspec(naked) HWND __fastcall TWinControl_GetHandle(LPCVOID WinControl)
{
	__asm
	{
		mov     edx, 0058750CH
		mov     eax, ecx
		jmp     edx
	}
}
