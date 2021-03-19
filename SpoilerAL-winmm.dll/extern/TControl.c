#include "TControl.h"

extern const DWORD vcl_System_AnsiString_ctor_sz;
extern const DWORD vcl_System_AnsiString_dtor;

__declspec(naked) void __fastcall TControl_SetLeft(TControl *this, int Left)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 005803E4H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TControl_SetTop(TControl *this, int Top)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 00580408H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TControl_SetWidth(TControl *this, int Width)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 00580430H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TControl_SetHeight(TControl *this, int Height)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 00580454H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TControl_SetHelpContext(TControl *this, THelpContext HelpContext)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 005805DCH
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TControl_SetHelpKeyword(TControl *this, LPCSTR HelpKeyword)
{
	extern const DWORD _TControl_SetHelpKeyword;

	__asm
	{
		push    eax
		mov     eax, esp
		push    ecx
		call    dword ptr [vcl_System_AnsiString_ctor_sz]
		mov     edx, dword ptr [eax]
		pop     eax
		call    dword ptr [_TControl_SetHelpKeyword]
		mov     edx, 2
		mov     eax, esp
		call    dword ptr [vcl_System_AnsiString_dtor]
		pop     eax
		ret
	}
}

__declspec(naked) void __fastcall TControl_SetCursor(TControl *this, TCursor Cursor)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 00580EA8H
		jmp     ecx
	}
}
