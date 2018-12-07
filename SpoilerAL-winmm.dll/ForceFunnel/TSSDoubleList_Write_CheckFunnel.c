#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSDoubleList_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC (ebp + 0CH)
		#define SSGS (ebp + 8H)

		test    eax, eax
		jnz     SKIP
		mov     edx, dword ptr [SSGS]
		mov     ecx, dword ptr [SSGC]
		push    dword ptr [ebp - 160H]// Index
		push    edx
		push    ecx
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
	SKIP:
		dec     dword ptr [ebx + 1CH]
		ret

		#undef SSGC
		#undef SSGS
	}
}

