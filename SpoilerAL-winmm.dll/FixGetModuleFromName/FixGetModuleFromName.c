#include "TProcessCtrl.h"

static void __fastcall ProcessCheck(TProcessCtrl *this)
{
	HANDLE hProcess = TProcessCtrl_Open(this, PROCESS_QUERY_INFORMATION);
	if (hProcess)
		CloseHandle(hProcess);
}

__declspec(naked) void __cdecl FixGetModuleFromName()
{
	static DWORD const TStringDivision_Upper = 0x004AE590;

	__asm
	{
		#define ProcessCtrl eax

		mov     ecx, ProcessCtrl
		call    ProcessCheck
		jmp     TStringDivision_Upper

		#undef ProcessCtrl
	}
}
