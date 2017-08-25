#include <windows.h>

extern const DWORD _TWinControl_GetHandle;

__declspec(naked) void __cdecl TGuideForm_ctor()
{
	static const char lpszString[] = "→ SSGデバッグモード - マウスオーバーを除く";

	__asm
	{
		#define _this                              (ebp - 4H)
		#define offsetof_TGuideForm_UserModeCmbBox 764
		#define ReturnAddress                      0048C247H

		pop     ecx
		add     dword ptr [ebp - 0CH], 4

		push    offset lpszString
		push    0
		mov     eax, dword ptr [_this]
		push    CB_ADDSTRING
		mov     eax, dword ptr [eax + offsetof_TGuideForm_UserModeCmbBox]
		call    dword ptr [_TWinControl_GetHandle]
		push    eax
		push    ReturnAddress
		jmp     SendMessageA

		#undef _this
		#undef offsetof_TGuideForm_UserModeCmbBox
		#undef ReturnAddress
	}
}
