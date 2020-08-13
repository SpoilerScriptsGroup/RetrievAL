#include <windows.h>
#include "intrinsic.h"

EXTERN_C void __fastcall TApplication_ActivateHint(LPVOID *this, LPPOINT CursorPos);

EXTERN_C void __cdecl Attach_HintWindow()
{
	// TApplication::ActivateHint
	/*
		mov     ecx, eax                                ; 005721C4 _ 8B. C8
		jmp     TApplication_ActivateHint               ; 005721C6 _ E9, ????????
		nop                                             ; 005721CB _ 90
		nop                                             ; 005721CC _ 90
	*/
	*(LPWORD )0x005721C4 = BSWAP16(0x8BC8);
	*(LPBYTE )0x005721C6 = 0xE9;
	*(LPDWORD)0x005721C7 = (DWORD)TApplication_ActivateHint - (0x005721C7 + sizeof(DWORD));
	*(LPWORD )0x005721CB = BSWAP16(0x9090);
}
