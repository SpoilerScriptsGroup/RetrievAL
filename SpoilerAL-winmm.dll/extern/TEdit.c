#include "TEdit.h"

__declspec(naked) void __fastcall TEdit_SetFocus(LPVOID Edit)
{
	__asm
	{
		mov     edx, dword ptr [ecx]
		mov     eax, ecx
		jmp     dword ptr [edx + 0C0H]
	}
}
