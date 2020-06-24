#include <windows.h>

double __cdecl ParsingDouble(void *this, void *SSGS, const void *Src, double Val);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl Caller_ParsingDouble()
{
	static const DWORD X005174C9 = 0x005174C9;

	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L1
		jmp     ParsingDouble

	L1:
		push    ebp
		mov     ebp, esp
		sub     esp, 2288
		jmp     dword ptr [X005174C9]
	}
}
