#include "TProcessCtrl.h"

__declspec(naked) void __cdecl FixGetModuleFromName()
{
	__asm
	{
		#define ProcessCtrl ecx

		cmp     dword ptr [ProcessCtrl + TProcessCtrl.entry.th32ProcessID], 0
		je      L1
		jmp     dword ptr [TProcessCtrl_LoadModuleList]

	L1:
		jmp     TProcessCtrl_Attach

		#undef ProcessCtrl
	}
}
