#include "TProcessCtrl.h"

__declspec(naked) void __cdecl FixGetModuleFromName()
{
	__asm
	{
		#define ProcessCtrl                              ecx
		#define offsetof_ProcessCtrl_entry_th32ProcessID 200

		cmp     dword ptr [ProcessCtrl + offsetof_ProcessCtrl_entry_th32ProcessID], 0
		je      L1
		jmp     dword ptr [TProcessCtrl_LoadModuleList]

	L1:
		jmp     TProcessCtrl_Attach

		#undef ProcessCtrl
		#undef offsetof_ProcessCtrl_entry_th32ProcessID
	}
}
