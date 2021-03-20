#include "TListBox.h"

__declspec(naked) int __fastcall TListBox_GetItemIndex(LPVOID ListBox)
{
	__asm
	{
		mov     edx, dword ptr [ecx]
		mov     eax, ecx
		jmp     dword ptr [edx + 0C8H]
	}
}

__declspec(naked) int __fastcall TListBox_GetTopIndex(LPVOID ListBox)
{
	__asm
	{
		mov     edx, 00545D60H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) int __fastcall TListBox_GetItemHeight(LPVOID ListBox)
{
	__asm
	{
		mov     edx, 00545B74H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) int __fastcall TListBox_ItemAtPos(LPVOID ListBox, LPPOINT Pos, BOOLEAN Existing)
{
	__asm
	{
		push    ecx
		mov     eax, dword ptr [esp + 4]
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], 00545F58H
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}
