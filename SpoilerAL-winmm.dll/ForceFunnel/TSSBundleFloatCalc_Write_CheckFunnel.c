#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSBundleFloatCalc_Write_CheckFunnel()
{
	extern BOOL FixTheProcedure;
	__asm
	{
		#define SSGC edi
		#define SSGS esi

		lea     eax, [ebp - 0x84]// Val
		cmp     FixTheProcedure, 0
		je      SKIP
		mov     eax, [ebp + 0x10]// Arg
		lea     eax, [eax + 0x08]// TSSArgDouble*->value
	SKIP:
		push    eax
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

