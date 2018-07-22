#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSDoubleList_WriteOne_CheckFunnel()
{
	__asm
	{
		#define ReturnAddress 004C5512H
		#define SSGC          (ebp + 0CH)
		#define SSGS          (ebp + 8H)

		mov     edx, dword ptr [SSGS]
		mov     ecx, dword ptr [SSGC]
		push    ReturnAddress
		push    dword ptr [ebp - 160H]// Index
		push    edx
		push    ecx
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		xor     eax, eax
		ret

		#undef ReturnAddress
		#undef SSGC
		#undef SSGS
	}
}

