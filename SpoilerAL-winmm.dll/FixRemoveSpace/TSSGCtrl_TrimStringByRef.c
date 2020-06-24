#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

__declspec(naked) void __cdecl TSSGCtrl_TrimStringByRef()
{
	static const DWORD X004FFDA2 = 0x004FFDA2;

	__asm
	{
		lea     ecx, [ebp - 18H]
		call    string_trim_blank
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		jmp     dword ptr [X004FFDA2]
	}
}
