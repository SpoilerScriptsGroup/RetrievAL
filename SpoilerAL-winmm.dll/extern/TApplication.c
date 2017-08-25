#include "TApplication.h"

__declspec(naked) void __fastcall TApplication_ProcessMessages(LPVOID _Application)
{
	__asm
	{
		mov     edx, 00571120H
		mov     eax, ecx
		jmp     edx
	}
}
