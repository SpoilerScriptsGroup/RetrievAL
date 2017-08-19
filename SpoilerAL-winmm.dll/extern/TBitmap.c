#include "TBitmap.h"

__declspec(naked) LPVOID __fastcall TBitmap_GetCanvas(LPVOID Bitmap)
{
	__asm
	{
		mov     edx, 00562CF0H
		mov     eax, ecx
		jmp     edx
	}
}
