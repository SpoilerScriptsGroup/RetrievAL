#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSFloatCalc_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC esi
		#define SSGS ebx

		push    0
		push    SSGS
		push    SSGC
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		dec     dword ptr [ebp - 0CH]
		ret

		#undef SSGC
		#undef SSGS
	}
}

