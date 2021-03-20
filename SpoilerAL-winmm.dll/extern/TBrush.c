#include "TBrush.h"

__declspec(naked) TBrushStyle __fastcall TBrush_GetStyle(TBrush *this)
{
	__asm
	{
		mov     ecx, dword ptr [ecx + 16]
		xor     eax, eax
		mov     al, byte ptr [ecx + 28]
		ret
	}
}

__declspec(naked) void __fastcall TBrush_SetStyle(TBrush *this, TBrushStyle Style)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055DC58H
		jmp     ecx
	}
}

__declspec(naked) COLORREF __fastcall TBrush_GetColor(TBrush *this)
{
	__asm
	{
		mov     eax, dword ptr [ecx + 16]
		mov     eax, dword ptr [eax + 20]
		ret
	}
}

__declspec(naked) void __fastcall TBrush_SetColor(TBrush *this, COLORREF Color)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055DB3CH
		jmp     ecx
	}
}
