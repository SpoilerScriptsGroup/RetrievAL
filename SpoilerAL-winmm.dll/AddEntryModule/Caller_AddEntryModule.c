#include "TProcessCtrl.h"

extern void __stdcall AddEntryModule(void *moduleList, unsigned long th32ProcessID);

__declspec(naked) void __cdecl Caller_AddEntryModule()
{
	__asm
	{
		mov     eax, dword ptr [ebp + 8]
		sub     dword ptr [ebp - 56], 2

		#define this          eax
		#define ReturnAddress 004A50DDH

		mov     ecx, dword ptr [this + TProcessCtrl.entry.th32ProcessID]
		add     eax, offset TProcessCtrl.moduleList
		push    ecx
		push    eax
		push    ReturnAddress
		jmp     AddEntryModule

		#undef this
		#undef ReturnAddress
	}
}
