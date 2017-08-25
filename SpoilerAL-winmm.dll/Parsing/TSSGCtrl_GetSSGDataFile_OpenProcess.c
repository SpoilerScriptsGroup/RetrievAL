#include <windows.h>
#include "TProcessCtrl.h"

HANDLE __stdcall TSSGCtrl_OpenProcess(TProcessCtrl *processCtrl, DWORD Mode, LPCSTR addressStr);

__declspec(naked) void __cdecl TSSGCtrl_GetSSGDataFile_OpenProcess()
{
	__asm
	{
		#define StepSize (ebp - 308H)
		#define SHandle  (ebp - 350H)

		mov     eax, dword ptr [StepSize]
		mov     ecx, dword ptr [ebp - 30H]
		test    eax, eax
		jz      L2
		mov     eax, dword ptr [ebp + 8H]
		mov     ecx, dword ptr [ecx + 18H]
		add     eax, 432
		push    ecx
		push    16
		push    eax
		call    TSSGCtrl_OpenProcess
		test    eax, eax
		jz      L1
		mov     dword ptr [SHandle], eax
		jmp     L2
	L1:
		mov     dword ptr [StepSize], eax
	L2:
		ret

		#undef StepSize
		#undef SHandle
	}
}
