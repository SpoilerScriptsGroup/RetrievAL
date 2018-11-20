#define USING_NAMESPACE_BCB6_STD
#include "TCalcImage.h"

void(__cdecl * const TCalcImage_SetStatus)(TCalcImage *this, long Index, BYTE Status) = (LPVOID)0x00483678;
BYTE(__cdecl * const TCalcImage_GetStatus)(TCalcImage *this, long Index) = (LPVOID)0x004836B4;

__declspec(naked) void __fastcall TCalcImage_DrawBtn(TCalcImage *this, long No, BOOLEAN IsPress)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    ecx
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], 0048292CH
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}

#define TCalcImage_FOnMouseUp(this, Sender, Button, Shift, X, Y) _TCalcImage_FOnMouseUp(this, Sender, Button, Y, X, Shift)
__declspec(naked) void __fastcall _TCalcImage_FOnMouseUp(TCalcImage *this, LPVOID Sender, int Button, int Y, int X, WORD Shift)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    ecx
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], 00482F5CH
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}

#define TCalcImage_FOnMouseDown(this, Sender, Button, Shift, X, Y) _TCalcImage_FOnMouseDown(this, Sender, Button, Y, X, Shift)
__declspec(naked) void __fastcall _TCalcImage_FOnMouseDown(TCalcImage *this, LPVOID Sender, int Button, int Y, int X, WORD Shift)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    ecx
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], 00482FD4H
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}
