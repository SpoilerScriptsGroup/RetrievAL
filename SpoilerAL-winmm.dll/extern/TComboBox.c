#include "TComboBox.h"

__declspec(naked) void __fastcall TComboBox_SetItemIndex(LPCVOID ComboBox, const int Index)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, dword ptr [ecx]
		jmp     dword ptr [ecx + 204]
	}
}
