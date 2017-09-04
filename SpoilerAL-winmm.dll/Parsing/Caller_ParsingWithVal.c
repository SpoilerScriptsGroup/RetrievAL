#include <windows.h>

unsigned long __cdecl Parsing(void *this, void *SSGS, const void *Src, ...);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl Caller_ParsingWithVal()
{
	static const char lpszVal[] = "Val";

	__asm
	{
		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, 005113F8H
		test    eax, eax
		jz      L1
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
		jmp     ecx
	}
}
