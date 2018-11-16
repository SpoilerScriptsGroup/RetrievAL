#include "TBrush.h"

__declspec(naked) void __fastcall TBrush_SetColor(LPVOID this, COLORREF color)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055DB3CH
		jmp     ecx
	}
}
