#include <windows.h>
#include "SnapWindow.h"

extern const DWORD _TWinControl_GetHandle;

__declspec(naked) void __cdecl TMainForm_ctor_AttachSnapWindow()
{
	__asm
	{
		#define _this (ebp - 4H)

		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		mov     eax, dword ptr [_this]
		call    dword ptr [_TWinControl_GetHandle]
		mov     ecx, eax
		jmp     AttachSnapWindow

		#undef _this
	}
}
