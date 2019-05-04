#include <windows.h>
#include "intrinsic.h"

EXTERN_C BOOL __cdecl IsSingleInstance();

#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void __cdecl Attach_FixSingleInstance()
{
	// WinMain
	/*
		call    IsSingleInstance                        ; 0040173D _ E8, ????????
		test    eax, eax                                ; 00401742 _ 85. C0
		jnz     0040177AH                               ; 00401744 _ 75, 34
		jmp     00401758H                               ; 00401746 _ EB, 10
		nop                                             ; 00401748 _ 90
	*/
	*(LPBYTE )0x0040173D = CALL_REL32;
	*(LPDWORD)0x0040173E = (DWORD)IsSingleInstance - (0x0040173E + sizeof(DWORD));
	*(LPWORD )0x00401742 = BSWAP16(0x85C0);
	*(LPDWORD)0x00401744 = BSWAP32(0x7534EB10);
	*(LPBYTE )0x00401748 = NOP;
}
