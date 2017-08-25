#include <windows.h>

BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);

__declspec(naked) void __cdecl TSSGCtrl_Funneling_Write()
{
	__asm
	{
		#define TSSGCtrl_Write 0051C408H

		mov     ecx, dword ptr [ebp - 34H]
		push    TSSGCtrl_Write
		mov     ecx, dword ptr [ecx + 18H]
		call    TSSGCtrl_IsRemoteProcess
		test    al, al
		jnz     L1
		call    GetCurrentProcess
		mov     dword ptr [esp + 16], eax
	L1:
		ret

		#undef TSSGCtrl_Write
	}
}
