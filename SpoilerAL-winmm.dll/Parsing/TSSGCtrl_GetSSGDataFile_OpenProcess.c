#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

extern HANDLE __stdcall TSSGCtrl_OpenProcess(TProcessCtrl *processCtrl, DWORD Mode, LPCSTR addressStr);

__declspec(naked) void __cdecl TSSGCtrl_GetSSGDataFile_OpenProcess(
	bcb6_std_string *ErrorStr,
	bcb6_std_string *tmpV_6)// = ecx
{
	__asm
	{
		#define SSGC     (ebp + 0x08)
		#define StepSize (ebp - 0x0308)
		#define SHandle  (ebp - 0x0350)

		call    string_trim_blank
		cmp     dword ptr [StepSize], 0
		je      L2

		mov     ecx, dword ptr [SSGC]
		lea     ecx, [ecx + TSSGCtrl.processCtrl]
		push    dword ptr [eax - size string * 5]
		push    PROCESS_VM_READ
		push    ecx
		call    TSSGCtrl_OpenProcess
		lea     edx, [SHandle]
		lea     ecx, [StepSize]
		test    eax, eax
		cmovz   edx, ecx
		mov     dword ptr [edx], eax
	L2:
		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		jmp     string_ctor_assign// construct ErrorStr
			
		#undef SHandle
		#undef StepSize
		#undef SSGC
	}
}
