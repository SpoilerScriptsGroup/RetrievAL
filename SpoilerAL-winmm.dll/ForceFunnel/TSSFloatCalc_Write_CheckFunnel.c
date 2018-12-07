#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSFloatCalc_Write_CheckFunnel()
{
	extern BOOL FixTheProcedure;
	__asm
	{
		#define SSGC esi
		#define SSGS ebx

		lea     eax, [ebp - 0x38]// Num
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
		dec     dword ptr [ebp - 0CH]
		ret

		#undef SSGC
		#undef SSGS
	}
}

