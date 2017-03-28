#include <windows.h>

EXTERN_C void __cdecl TSSBitList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSBundleList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSDoubleList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSGCtrl_OneRead_with_CheckIO_FEP();
EXTERN_C void __cdecl TSSGCtrl_OneWrite_with_CheckIO_FEP();

#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void Attach_FixListFEP()
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

	// TSSBundleList::Setting
	*(LPBYTE )0x004BEBDC = CALL_REL32;
	*(LPDWORD)0x004BEBDD = (DWORD)TSSBundleList_Setting_CheckFEPParam - (0x004BEBDD + sizeof(DWORD));
	*(LPBYTE )0x004BEBE1 = NOP;

	// TSSBundleList::Read
	// replaced at "Parsing\TSSBundleList_Read_OneRead.asm"

	// TSSBundleList::Write
	// replaced at "Parsing\TSSBundleList_Write_OneWrite.asm"

	// TSSDoubleList::Setting
	*(LPBYTE )0x004C372A = CALL_REL32;
	*(LPDWORD)0x004C372B = (DWORD)TSSDoubleList_Setting_CheckFEPParam - (0x004C372B + sizeof(DWORD));
	*(LPBYTE )0x004C372F = NOP;

	// TSSDoubleList::Read
	// replaced at "Parsing\TSSDoubleList_Read_OneRead.asm"

	// TSSDoubleList::Write
	// replaced at "Parsing\TSSDoubleList_Write_OneWrite.asm"

	// TSSList::Setting
	*(LPBYTE )0x00529A16 = CALL_REL32;
	*(LPDWORD)0x00529A17 = (DWORD)TSSList_Setting_CheckFEPParam - (0x00529A17 + sizeof(DWORD));
	*(LPBYTE )0x00529A1B = NOP;

	// TSSList::Read
	*(LPDWORD)(0x0052A15A + 1) = (DWORD)TSSGCtrl_OneRead_with_CheckIO_FEP - (0x0052A15A + 1 + sizeof(DWORD));

	// TSSList::Write
	*(LPDWORD)(0x0052A30C + 1) = (DWORD)TSSGCtrl_OneWrite_with_CheckIO_FEP - (0x0052A30C + 1 + sizeof(DWORD));
}
