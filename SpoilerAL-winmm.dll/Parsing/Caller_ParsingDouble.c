#include <windows.h>

double __cdecl ParsingDouble(void *this, void *SSGS, const void *Src, double Val);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl Caller_ParsingDouble()
{
	static const DWORD X005174C5 = 0x005174C5;

	__asm
	{
		cmp     EnableParserFix, FALSE
#pragma warning(suppress: 4414)
		jne     ParsingDouble
		jmp     X005174C5
		ud2
	}
}
