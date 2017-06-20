#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSBundleFloatCalc_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC edi
		#define SSGS esi

		push    0
		push    SSGS
		push    SSGC
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		dec     dword ptr [ebp - 54H]
		ret

		#undef SSGC
		#undef SSGS
	}
}

