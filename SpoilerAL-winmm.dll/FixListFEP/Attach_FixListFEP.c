#include <windows.h>
#include "TSSCalc.h"

EXTERN_C void __cdecl TSSBitList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSBundleList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSDoubleList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSGCtrl_OneRead_with_CheckIO_FEP();
EXTERN_C void __cdecl TSSGCtrl_OneWrite_with_CheckIO_FEP();

static __declspec(naked) unsigned long __fastcall TSSBundleCalc_Read_TotalVal(unsigned __rep, unsigned long Val)
{
	__asm {
		cmp   [ebx]TSSCalc.isUnsigned, 0
		jne   UNSIGNED

		movzx ecx, [ebx]TSSCalc.size

		movsx eax, dl
		dec   ecx// size == 1
		cmovz edx, eax

		movsx eax, dx
		dec   ecx// size == 2
		cmovz edx, eax

	UNSIGNED:
		push  FALSE
		push  edx
		push  ebx
		push  dword ptr [ebp + 0x0C]
		call  TSSGCtrl_CheckIO_FEP
		add   esp, 0x10
		add   dword ptr [ebp - 0xAC], eax
		ret
	}
}

#define PUSH_EAX   (BYTE)0x50
#define PUSH_EBX   (BYTE)0x53
#define PUSH_IMM8  (BYTE)0x6A
#define NOP        (BYTE)0x90
#define NOP_X4    (DWORD)0x00401F0F
#define CDQ        (BYTE)0x99
#define CALL_REL32 (BYTE)0xE8
#define JMP_REL32  (BYTE)0xE9
#define JMP_SHORT  (BYTE)0xEB

EXTERN_C void __cdecl Attach_FixListFEP()
{
	// TSSBitList::Setting
	*(LPBYTE )0x004B8352 = CALL_REL32;
	*(LPDWORD)0x004B8353 = (DWORD)TSSBitList_Setting_CheckFEPParam - (0x004B8353 + sizeof(DWORD));
	*(LPBYTE )0x004B8357 = NOP;

	// TSSBitList::Read
	*(LPDWORD)(0x004B910F + 1) = (DWORD)TSSGCtrl_OneRead_with_CheckIO_FEP - (0x004B910F + 1 + sizeof(DWORD));

	// TSSBitList::Write
	*(LPDWORD)(0x004BB522 + 1) = (DWORD)TSSGCtrl_OneRead_with_CheckIO_FEP - (0x004BB522 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004BB6F9 + 1) = (DWORD)TSSGCtrl_OneWrite_with_CheckIO_FEP - (0x004BB6F9 + 1 + sizeof(DWORD));

	// TSSBundleCalc::Read
	*(LPBYTE ) 0x004BDE16      = NOP;
	*(LPBYTE )(0x004BDE17 + 0) = CALL_REL32;
	*(LPDWORD)(0x004BDE17 + 1) = (DWORD)TSSBundleCalc_Read_TotalVal - (0x004BDE17 + 1 + sizeof(DWORD));

	*(LPWORD ) 0x004BDF1E      = BSWAP16(NOP << 8 | CDQ);
	*(LPBYTE )(0x004BDF20 + 1) = 0xF9;

	*(LPBYTE )(0x004BDF28 + 0) = JMP_SHORT;
	*(LPBYTE )(0x004BDF28 + 1) = 0x004BDF38 - (0x004BDF28 + 1 + sizeof(BYTE));

	// TSSBundleList::Setting
	*(LPBYTE )0x004BEBDC = CALL_REL32;
	*(LPDWORD)0x004BEBDD = (DWORD)TSSBundleList_Setting_CheckFEPParam - (0x004BEBDD + sizeof(DWORD));
	*(LPBYTE )0x004BEBE1 = NOP;

	// TSSBundleList::Read
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSBundleList::Write
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSBundleList::Read
	*(LPWORD )0x004BF5F2 = BSWAP16(NOP << 8 | CDQ);
	*(LPBYTE )0x004BF5F5 = 0x7D;
	*(LPWORD )0x004BF5FA = BSWAP16(NOP << 8 | CDQ);
	*(LPBYTE )0x004BF5FD = 0xBE;

	// TSSDoubleList::Setting
	*(LPBYTE )0x004C372A = CALL_REL32;
	*(LPDWORD)0x004C372B = (DWORD)TSSDoubleList_Setting_CheckFEPParam - (0x004C372B + sizeof(DWORD));
	*(LPBYTE )0x004C372F = NOP;

	// TSSDoubleList::Read
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSDoubleList::Write
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSList::Setting
	*(LPBYTE )0x00529A16 = CALL_REL32;
	*(LPDWORD)0x00529A17 = (DWORD)TSSList_Setting_CheckFEPParam - (0x00529A17 + sizeof(DWORD));
	*(LPBYTE )0x00529A1B = NOP;

	// TSSList::Read
	*(LPDWORD)(0x0052A15A + 1) = (DWORD)TSSGCtrl_OneRead_with_CheckIO_FEP - (0x0052A15A + 1 + sizeof(DWORD));

	// TSSList::Write
	*(LPDWORD)(0x0052A30C + 1) = (DWORD)TSSGCtrl_OneWrite_with_CheckIO_FEP - (0x0052A30C + 1 + sizeof(DWORD));

	// TSSBundleFloatCalc::Read
	*(LPBYTE )(0x0052E482 + 0) = JMP_REL32;
	*(LPDWORD)(0x0052E482 + 1) = 0x0052E5B4 - (0x0052E482 + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x0052E487      = NOP;

	*(LPDWORD)(0x0052E5A6 + 2) = -0x0190;
	*(LPBYTE )(0x0052E5B2 + 0) = JMP_SHORT;
	*(LPBYTE )(0x0052E5B2 + 1) = 0x0052E5EB - (0x0052E5B2 + 1 + sizeof(BYTE));

	*(LPBYTE )(0x0052E5B4 + 0) = PUSH_IMM8;
	*(LPBYTE )(0x0052E5B4 + 1) = FALSE;
	*(LPWORD )(0x0052E5B6 + 0) = BSWAP16(0xDD85);// fld qword ptr [ebp + ...
	*(LPDWORD)(0x0052E5B6 + 2) =        -0x0138;
	*(LPWORD ) 0x0052E5BC      = PUSH_EAX << 8 | PUSH_EAX;
	*(LPWORD )(0x0052E5BE + 0) = BSWAP16(0xDD1C);// fstp
	*(LPBYTE )(0x0052E5BE + 2) =         0x24   ;// qword ptr [esp]
	*(LPWORD ) 0x0052E5C1      = PUSH_EBX;
	*(LPWORD )(0x0052E5C2 + 0) = BSWAP16(0xFF75);// push dword ptr [ebp + ...
	*(LPBYTE )(0x0052E5C2 + 2) =         0x0C   ;
	*(LPWORD )(0x0052E5CA + 0) = BSWAP16(0x83C4);// add esp, ...
	*(LPWORD )(0x0052E5CA + 2) =         0x14;
	*(LPBYTE )(0x0052E5CD + 0) = JMP_REL32;
	*(LPDWORD)(0x0052E5CD + 1) = 0x0052E488 - (0x0052E5CD + 1 + sizeof(DWORD));
	*(LPDWORD) 0x0052E5D2      = NOP_X4;
}
