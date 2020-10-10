#include "TStringDivision.h"

__declspec(naked) void __cdecl FixAdjustByValue()
{
	__asm
	{
		mov     cl, byte ptr [ebx + 78H]
		sub     cl, 4
		jae     L1
		neg     cl
		shl     cl, 3
		shl     eax, cl
	L1:
		bswap   eax
		mov     dword ptr [esp + 8], eax
		jmp     TStringDivision_ToString
	}
}
