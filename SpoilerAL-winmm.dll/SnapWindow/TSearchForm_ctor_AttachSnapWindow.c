#include <windows.h>
#include "SnapWindow.h"

#pragma warning(disable:4733)

extern const DWORD _TWinControl_GetHandle;

__declspec(naked) void __cdecl TSearchForm_ctor_AttachSnapWindow()
{
	__asm
	{
		#define _this (ebp - 4H)

		mov     eax, dword ptr [_this]
		call    dword ptr [_TWinControl_GetHandle]
		mov     ecx, eax
		call    AttachSnapWindow
		mov     eax, dword ptr [ebp - 40H]
        mov     dword ptr fs:[0], eax
		ret

		#undef _this
	}
}
