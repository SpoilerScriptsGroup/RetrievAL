#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

void __stdcall ReplaceDefineDynamic(void *SSGS, void *line);

extern BOOL EnableParserFix;
extern const DWORD F00504284;

__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_ReplaceDefineDynamic1()
{
	__asm
	{
		#define CallAddress 00504284H
		#define SSGS        (ebp + 10H)
		#define NameStr     edx

		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, dword ptr [SSGS]
		test    eax, eax
		jz      L1
		push    NameStr
		push    ecx
		push    CallAddress
		jmp     ReplaceDefineDynamic
	L1:
		jmp     dword ptr [F00504284]

		#undef CallAddress
		#undef SSGS
		#undef NameStr
	}
}

__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_ReplaceDefineDynamic2()
{
	__asm
	{
		#define SSGS         (ebp + 10H)
		#define ReturnString eax

		mov     ecx, dword ptr [EnableParserFix]
		mov     edx, dword ptr [SSGS]
		test    ecx, ecx
		jz      L1
		push    ReturnString
		push    edx
		call    ReplaceDefineDynamic
	L1:
		mov     edx, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 4]
		jmp     string_ctor_assign

		#undef SSGS
		#undef ReturnString
	}
}
