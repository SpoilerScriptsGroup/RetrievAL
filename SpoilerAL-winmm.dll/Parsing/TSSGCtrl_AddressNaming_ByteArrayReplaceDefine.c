#include "TStringDivision.h"

void __stdcall ReplaceDefineDynamic(void *SSGS, void *line);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_ReplaceDefineDynamic3()
{
	__asm
	{
		#define SSGS (ebp + 10H)
		#define tmpS eax

		mov     ecx, dword ptr [EnableParserFix]
		mov     edx, dword ptr [SSGS]
		test    ecx, ecx
		jz      L1
		push    tmpS
		push    edx
		push    offset TStringDivision_Half
		jmp     ReplaceDefineDynamic

	L1:
		jmp     TStringDivision_Half

		#undef SSGS
		#undef tmpS
	}
}
