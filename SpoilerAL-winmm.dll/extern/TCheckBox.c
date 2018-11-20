#include "TCheckBox.h"

__declspec(naked) void __fastcall TCheckBox_SetChecked(LPVOID this, BOOLEAN Value)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, dword ptr [ecx]
		jmp     dword ptr [ecx + 200]
	}
}

__declspec(naked) BOOLEAN __fastcall TCheckBox_GetChecked(LPVOID this)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, dword ptr [ecx]
		jmp     dword ptr [ecx + 196]
	}
}
