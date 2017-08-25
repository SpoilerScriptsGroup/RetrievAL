#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

void __fastcall TrimString(string *s);

__declspec(naked) void __cdecl TSSGCtrl_GetSSGDataFile_TrimString()
{
	__asm
	{
		push    004166F0H
		jmp     TrimString
	}
}
