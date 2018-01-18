#include <windows.h>
#include "bcb6_std_string.h"

extern BOOL FixTheProcedure;

__declspec(naked) BOOLEAN __cdecl TSSGCtrl_Funneling_IsEnabled(void *TSSGCtrl, void *SSGS)
{
	__asm
	{
		mov     eax, dword ptr [FixTheProcedure]
		test    al, al
		jz      IsEnabled
		ret

	IsEnabled:
		mov     eax, 0x005111E0
		jmp     eax
	}
}

__declspec(naked) unsigned long __cdecl TSSGCtrl_Funneling_GetAddress(void *TSSGCtrl, void *SSGS, void *AddressStr, unsigned long Mode)
{
	__asm
	{
		mov     eax, dword ptr [FixTheProcedure]
		test    eax, eax
		jz      GetAddress
		ret

	GetAddress:
		mov     eax, 0x00503920
		jmp     eax
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
		mov     eax, dword ptr[FixTheProcedure]
		test    eax, eax
		jz      MakeDataCode
		push    0x0F                            // Mode = ssgCtrl::atALL
		mov     ecx, dword ptr [ebp - 0x34]     // tmpV
		lea     edx, [ecx + 0x18]               // tmpV[1]
		push    edx                             // AddressStr = tmpV[1]
		push    dword ptr [ebp + 0x0C]          // SSGS
		push    edi                             // this
		mov     eax, 0x00503920                 // TSSGCtrl::GetAddress
		call    eax
		add     esp, 0x10
		mov     dword ptr [ebp - 0x0148], eax   // Address = eax
		mov     dword ptr [esp + 0x30], eax     // StartAddress = eax
	MakeDataCode:
		mov     eax, 0x0050F600
		jmp     eax
	}
}
