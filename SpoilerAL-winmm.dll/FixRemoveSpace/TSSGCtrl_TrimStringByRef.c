#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

__declspec(naked) void __cdecl TSSGCtrl_TrimStringByRef()
{
	__asm
	{
		lea     ecx, [ebp - 18H]
		call    string_trim_blank
		mov     eax, 004FFDA2H
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		jmp     eax
	}
}
