#include "TEdit.h"

__declspec(naked) void __fastcall TEdit_SetFocus(LPVOID Edit)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, dword ptr [ecx]
		jmp     dword ptr [ecx + 0C0H]
	}
}
