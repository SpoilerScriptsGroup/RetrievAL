#include <windows.h>

#if 0
unsigned long __cdecl Parsing61(void *SSGCtrl, void *SSGS, const void *Src, unsigned long Val);
#else
unsigned long __cdecl Parsing(void *this, void *SSGS, const void *Src, ...);
#endif

extern BOOL EnableParserFix;
extern const DWORD F005113F8;

__declspec(naked) void __cdecl Caller_Parsing()
{
	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L1
#if 0
		jmp     Parsing61
#else
		jmp     Parsing
#endif

	L1:
		jmp     dword ptr [F005113F8]
	}
}
