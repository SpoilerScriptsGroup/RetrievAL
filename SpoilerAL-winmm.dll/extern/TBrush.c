#include "TBrush.h"


__declspec(naked) COLORREF __fastcall TBrush_GetColor(TBrush *this)
{
	__asm
	{
		mov     eax, dword ptr [ecx + 16]
		mov     eax, dword ptr [eax + 20]
		ret
	}
}

__declspec(naked) void __fastcall TBrush_SetColor(TBrush *this, COLORREF color)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055DB3CH
		jmp     ecx
	}
}
