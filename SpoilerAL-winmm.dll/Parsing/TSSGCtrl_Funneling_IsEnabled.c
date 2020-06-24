#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

extern BOOL FixTheProcedure;
extern const DWORD F0050F600;
extern const DWORD F005111E0;

__declspec(naked) BOOLEAN __cdecl TSSGCtrl_Funneling_IsEnabled(void *TSSGCtrl, void *SSGS)
{
	__asm
	{
		cmp     dword ptr [FixTheProcedure], 0
		je      IsEnabled
		ret

	IsEnabled:
		jmp     dword ptr [F005111E0]
	}
}

__declspec(naked) unsigned long __cdecl TSSGCtrl_Funneling_GetAddress(void *TSSGCtrl, void *SSGS, void *AddressStr, unsigned long Mode)
{
	__asm
	{
		cmp     dword ptr [FixTheProcedure], 0
		je      GetAddress
		ret

	GetAddress:
		jmp     dword ptr [TSSGCtrl_GetAddress]
	}
}

__declspec(naked) void __cdecl TSSGCtrl_Funneling_MakeDataCode(
	void *            vector,
	void *            TSSGCtrl,
	void *            SSGS,
	bcb6_std_string   OnCode,
	bcb6_std_string * OffCode,
	BOOLEAN           IsOn,
	LPCVOID           StartAddress,
	ptrdiff_t         OffsetAddress)
{
	__asm
	{
		cmp     dword ptr [FixTheProcedure], 0
		je      MakeDataCode
		push    0x0F                            // Mode = ssgCtrl::atALL
		mov     ecx, dword ptr [ebp - 0x34]     // tmpV
		lea     edx, [ecx + 0x18]               // tmpV[1]
		push    edx                             // AddressStr = tmpV[1]
		push    dword ptr [ebp + 0x0C]          // SSGS
		push    edi                             // this
		call    dword ptr [TSSGCtrl_GetAddress] // TSSGCtrl::GetAddress
		add     esp, 0x10
		mov     dword ptr [ebp - 0x0148], eax   // Address = eax
		mov     dword ptr [esp + 0x30], eax     // StartAddress = eax
	MakeDataCode:
		jmp     dword ptr [F0050F600]
	}
}
