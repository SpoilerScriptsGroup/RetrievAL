#include <windows.h>

double __cdecl ParsingDouble(void *_this, void *SSGS, const void *Src, double Val);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl Caller_ParsingDouble()
{
	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L1
		jmp     ParsingDouble
	L1:
		push    ebp
		mov     eax, 005174C0H + 9
		mov     ebp, esp
		sub     esp, 2288
		jmp     eax
	}
}
