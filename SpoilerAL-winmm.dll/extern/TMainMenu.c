#include "TMainMenu.h"

__declspec(naked) HMENU __fastcall TMainMenu_GetHandle(LPCVOID MainMenu)
{
	__asm
	{
		mov     edx, dword ptr [ecx]
		mov     eax, ecx
		jmp     dword ptr [edx + 52]
	}
}
