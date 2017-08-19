#include "TWinControl.h"

__declspec(naked) HWND __fastcall TWinControl_GetHandle(LPVOID WinControl)
{
	__asm
	{
		mov     edx, 0058750CH
		mov     eax, ecx
		jmp     edx
	}
}
