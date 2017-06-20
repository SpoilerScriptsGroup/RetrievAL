#include <windows.h>
#include "TSSGCtrl.h"

#define ssgCtrl_reNO_ERROR 0

__declspec(naked) void __cdecl TSSFloatCalc_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC esi
		#define SSGS ebx

		push    ssgCtrl_reNO_ERROR
		push    0
		push    SSGC
		push    SSGC
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		dec     dword ptr [ebp - 0CH]
		ret

		#undef SSGC
		#undef SSGS
	}
}

