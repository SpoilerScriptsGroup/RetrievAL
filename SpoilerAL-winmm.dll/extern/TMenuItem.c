#include "TMenuItem.h"

extern const DWORD vcl_System_AnsiString_ctor_sz;
extern const DWORD vcl_System_AnsiString_dtor;

__declspec(naked) HMENU __fastcall TMenuItem_GetHandle(TMenuItem *this)
{
	__asm
	{
		mov     edx, 0054A0DCH
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TMenuItem_SetCaption(TMenuItem *this, LPCSTR lpText)
{
	extern const DWORD _TMenuItem_SetCaption;

	__asm
	{
		push    eax
		mov     eax, esp
		push    ecx
		call    dword ptr [vcl_System_AnsiString_ctor_sz]
		mov     edx, dword ptr [eax]
		pop     eax
		call    dword ptr [_TMenuItem_SetCaption]
		mov     edx, 2
		mov     eax, esp
		call    dword ptr [vcl_System_AnsiString_dtor]
		pop     eax
		ret
	}
}
