#include <windows.h>

void __declspec(naked) __cdecl TSearchForm_AdjustValToString_GetStart(LPVOID activeElement, LPVOID ssgCtrl)
{
	static const DWORD x004D1858 = 0x004D1858;

	__asm
	{
		cmp     byte ptr [esi + 0x0400 - 0x34], 1
		jne     GetStart
		mov     eax, dword ptr [esi + 0x0400 + 0x14]
		ret

	GetStart:
		jmp     dword ptr [x004D1858]
	}
}
