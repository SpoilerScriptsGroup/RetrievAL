#include <windows.h>

#if 0
unsigned long __cdecl Parsing61(void *SSGCtrl, void *SSGS, const void *Src, unsigned long Val);
#else
unsigned long __cdecl Parsing(void *_this, void *SSGS, const void *Src, ...);
#endif

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl Caller_Parsing()
{
	__asm
	{
		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, 005113F8H
		test    eax, eax
		jz      L1
#if 0
		jmp     Parsing61
#else
		jmp     Parsing
#endif
	L1:
		jmp     ecx
	}
}
