#include <windows.h>

extern const DWORD F00401FD8;
extern const DWORD _TWinControl_GetHandle;

__declspec(naked) void __cdecl TGetSearchRangeForm_ctor()
{
	static const char data1[] = "対象プロセスの、ヒープ領域を取得";

	__asm
	{
		call    dword ptr [F00401FD8]
		add     dword ptr [ebp - 0CH], 16
		push    offset data1
		mov     edx, dword ptr [ebp - 4H]
		mov     eax, dword ptr [edx + 31CH]
		call    dword ptr [_TWinControl_GetHandle]
		push    eax
		push    0048B1BFH
		jmp     SetWindowTextA
	}
}
