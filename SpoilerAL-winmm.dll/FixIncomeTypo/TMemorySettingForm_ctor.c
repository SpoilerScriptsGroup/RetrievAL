#include <windows.h>

extern const DWORD F00401FD8;
extern const DWORD _TWinControl_GetHandle;

__declspec(naked) void __cdecl TMemorySettingForm_ctor()
{
	static const char data1[] = "ƒtƒ@ƒCƒ‹‚ÌCRC32‚ðŽæ“¾";

	__asm
	{
		call    dword ptr [F00401FD8]
		add     dword ptr [ebp - 0CH], 16
		push    offset data1
		mov     edx, dword ptr [ebp - 4H]
		mov     eax, dword ptr [edx + 308H]
		call    dword ptr [_TWinControl_GetHandle]
		push    eax
		push    0047B483H
		jmp     SetWindowTextA
	}
}
