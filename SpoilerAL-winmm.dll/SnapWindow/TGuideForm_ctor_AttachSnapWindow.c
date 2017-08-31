#include <windows.h>
#include "SnapWindow.h"

extern const DWORD _TWinControl_GetHandle;

__declspec(naked) void __cdecl TGuideForm_ctor_AttachSnapWindow()
{
	__asm
	{
		#define _this (ebp - 4H)

		mov     eax, dword ptr [_this]
		call    dword ptr [_TWinControl_GetHandle]
		mov     ecx, eax
		call    AttachSnapWindow
		add     dword ptr [ebp - 0CH], 4
		mov     ecx, dword ptr [ebp - 28H]
		ret

		#undef _this
	}
}
