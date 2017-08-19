#include "TApplication.h"

__declspec(naked) void __fastcall TApplication_ProcessMessages(LPVOID _Application)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 00571120H
		jmp     ecx
	}
}
