#include <windows.h>
#include "TSSGCtrl.h"

#define ssgCtrl_reNO_ERROR 0

__declspec(naked) void __cdecl TSSBundleFloatCalc_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC edi
		#define SSGS esi

		push    ssgCtrl_reNO_ERROR
		push    0
		push    SSGC
		push    SSGC
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		dec     dword ptr [ebp - 54H]
		ret

		#undef SSGC
		#undef SSGS
	}
}

