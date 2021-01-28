#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TProcessCtrl.h"

static BOOLEAN __cdecl JumpTable(
	HANDLE        const SHandle,
	unsigned long const Address,
	char         *const tmpC,
	unsigned long const DataSize)
{
	switch (DataSize)
	{
	case 4:
		*(LPDWORD)tmpC = Address;
		break;
	case 3:
		*(LPDWORD)tmpC = Address & 0x00FFFFFF;
		break;
	case 2:
		*(LPWORD )tmpC = (WORD)Address;
		break;
	case 1:
		*(LPBYTE )tmpC = (BYTE)Address;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

EXTERN_C __declspec(naked) BOOLEAN __fastcall TSSGCtrl_AddressNaming_OneRead(
	string const *const tmpV_0,
	unsigned long const DataSize
#if 0
	, HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size
#endif
)// static method
{
	__asm {
		mov   eax, [ecx]string._M_start
	L0:
		inc   eax
		movzx edx, [eax]
		cmp   edx, ' '
		je    L0
		sub   edx, '\t'
		cmp   edx, '\r' - '\t'
		jbe   L0

		lea   edx, [eax + 3]
		cmp   edx, [ecx]string._M_finish
		jne   L5
		cmp   dword ptr [eax], MASM_BSWAP32('fmt\0')
#pragma warning(suppress: 4414)
		je    JumpTable
	L5:
		jmp   TProcessCtrl_OneRead
		ud2
	}
}
