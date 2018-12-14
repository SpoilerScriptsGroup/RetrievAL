#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

extern const BOOL EnableParserFix;
extern const DWORD bcb6_std_string_substr;

unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

__declspec(naked) void __cdecl Caller_ParsingWithRel()
{
	static const char lpszRel[] = "Rel";

	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L1

		#define ReturnAddress 0050B51AH
		#define Rel           (ebp + 44)

		mov     eax, dword ptr [Rel]
		mov     ecx, dword ptr [ebp - 0D60H]
		push    0
		push    0
		push    eax
		push    offset lpszRel
		push    3
		lea     eax, [esi + 2]
		sub     ecx, eax
		lea     edx, [ebp - 3E8H]
		push    edx
		push    ecx
		push    eax
		lea     ecx, [ebp + 14H]
		push    ecx
		push    edx
		call    dword ptr [bcb6_std_string_substr]
		add     esp, 16
		mov     ecx, dword ptr [ebp + 10H]
		mov     eax, dword ptr [ebp + 0CH]
		push    ecx
		push    eax
		call    Parsing
		mov     ecx, ReturnAddress
		add     esp, 32
		jmp     ecx

		#undef ReturnAddress
		#undef Rel

		align   16
	L1:
		push    0
		mov     ecx, 0050B4D9H
		lea     eax, [esi + 2H]
		jmp     ecx
	}
}
