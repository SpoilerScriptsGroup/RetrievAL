#include <windows.h>
#include "TSSArg.h"
#include "TSSGCtrl.h"

__declspec(naked) struct
{
	bool   CheckFunnel;
	HANDLE SHandle;
} __cdecl TSSFloatCalc_Write_CheckFunnel(ResErr result)
{
	extern BOOL FixTheProcedure;
	__asm
	{
		#define Arg     (ebp + 0x10)
		#define SSGC     esi
		#define SSGS     ebx
		#define SHandle (ebp - 0x04)
		#define Num     (ebp - 0x38)

		lea     eax, [Num]
		cmp     FixTheProcedure, 0
		je      SKIP
		mov     ecx, [Arg]
		lea     eax, [ecx]TSSArgDouble.value
	SKIP:
		push    eax
		push    SSGS
		push    SSGC
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		mov     edx, dword ptr [SHandle]
		ret

		#undef Num
		#undef SHandle
		#undef SSGS
		#undef SSGC
		#undef Arg
	}
}

