#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

void __fastcall TrimString(string *s);

__declspec(naked) void __cdecl TSSGCtrl_TrimStringByRef()
{
	__asm
	{
		lea     ecx, [ebp - 18H]
		call    TrimString
		mov     eax, 004FFDA2H
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		jmp     eax
	}
}
