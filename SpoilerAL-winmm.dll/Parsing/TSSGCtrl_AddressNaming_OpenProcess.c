#include <windows.h>
#include "TProcessCtrl.h"

HANDLE __stdcall TSSGCtrl_OpenProcess(TProcessCtrl *processCtrl, DWORD Mode, LPCSTR addressStr);

__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_OpenProcess()
{
	__asm
	{
		#define ReturnAddress 00504F76H

		mov     ecx, dword ptr [esi]
		lea     eax, [edi + 1B0H]
		push    dword ptr [ecx + 18H]
		push    16
		push    eax
		push    ReturnAddress
		jmp     TSSGCtrl_OpenProcess

		#undef ReturnAddress
	}
}
