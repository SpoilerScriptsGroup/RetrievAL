#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSDoubleToggle_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC (ebp + 0CH)
		#define SSGS (ebp + 8H)

		mov     edx, dword ptr [SSGS]
		mov     ecx, dword ptr [SSGC]
		push    0
		push    edx
		push    ecx
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		ret

		#undef SSGC
		#undef SSGS
	}
}

