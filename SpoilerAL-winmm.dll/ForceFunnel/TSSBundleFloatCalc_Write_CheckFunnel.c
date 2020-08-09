#include <windows.h>
#include "TSSArg.h"
#include "TSSGCtrl.h"

__declspec(naked) struct
{
	bool   CheckFunnel;
	HANDLE SHandle;
} __cdecl TSSBundleFloatCalc_Write_CheckFunnel(ResErr result)
{
	extern BOOL FixTheProcedure;
	__asm
	{
		#define Arg     (ebp + 0x10)
		#define SSGC     edi
		#define SSGS     esi
		#define SHandle (ebp - 0x04)
		#define Val     (ebp - 0x84)

		lea     eax, [Val]
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

		#undef Val
		#undef SHandle
		#undef SSGS
		#undef SSGC
		#undef Arg
	}
}

