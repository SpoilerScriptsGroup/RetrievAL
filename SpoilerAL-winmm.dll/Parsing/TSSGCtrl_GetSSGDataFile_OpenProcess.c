#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TProcessCtrl.h"

HANDLE __stdcall TSSGCtrl_OpenProcess(TProcessCtrl *processCtrl, DWORD Mode, LPCSTR addressStr);

__declspec(naked) void __cdecl TSSGCtrl_GetSSGDataFile_OpenProcess(
	bcb6_std_string *ErrorStr,
	bcb6_std_string *tmpV_6)// = ecx
{
	__asm
	{
		#define StepSize (ebp - 308H)
		#define SHandle  (ebp - 350H)

		call    string_trim_blank
		cmp     dword ptr [StepSize], 0
		je      L2
		mov     ecx, dword ptr [ebp + 0x08]
		add     ecx, 0x01B0// processCtrl
		push    dword ptr [eax - 24 * 5]
		push    16
		push    ecx
		call    TSSGCtrl_OpenProcess
		lea     edx, [SHandle]
		lea     ecx, [StepSize]
		test    eax, eax
		cmovz   edx, ecx
		mov     dword ptr [edx], eax
	L2:
		mov     eax, 0x004166F0
		jmp     eax// construct ErrorStr

		#undef StepSize
		#undef SHandle
	}
}
