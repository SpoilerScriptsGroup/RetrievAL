#include <windows.h>

unsigned long __cdecl Parsing(void *this, void *SSGS, const void *Src, ...);

extern BOOL EnableParserFix;
extern const DWORD F005113F8;

__declspec(naked) void __cdecl Caller_ParsingWithVal()
{
	static const char lpszVal[] = "Val";

	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L1
		mov     eax, dword ptr [esp + 16]
		mov     edx, dword ptr [esp + 12]
		push    0
		push    0
		push    eax
		push    offset lpszVal
		mov     ecx, dword ptr [esp + 24]
		mov     eax, dword ptr [esp + 20]
		push    3
		push    edx
		push    ecx
		push    eax
		call    Parsing
		add     esp, 32
		ret

	L1:
		jmp     dword ptr [F005113F8]
	}
}
