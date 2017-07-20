#include <windows.h>

EXTERN_C void __cdecl TSSDoubleList_Read_SwitchStatus();
EXTERN_C void __cdecl TSSDoubleList_Write_SwitchStatus();
EXTERN_C void __cdecl TSSDoubleList_ToByteCode_SwitchStatus();
EXTERN_C void __cdecl TSSDoubleToggle_Read_SwitchStatus();
EXTERN_C void __cdecl TSSDoubleToggle_Write_SwitchStatus();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_ClearAtBreak();
EXTERN_C void __cdecl TSSGCtrl_SetSSGDataFile_IgnoreEmptyData();

#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x90909090
#define JMP_REL32 (BYTE )0xE9

EXTERN_C void __cdecl Attach_FixDoubleList()
{
	// TSSDoubleList::Setting
	*(LPBYTE )0x004C3A84 = JMP_REL32;
	*(LPDWORD)0x004C3A85 = 0x004C3E22 - (0x004C3A85 + sizeof(DWORD));

	// TSSDoubleList::Read
	*(LPBYTE )0x004C436D = JMP_REL32;
	*(LPDWORD)0x004C436E = (DWORD)TSSDoubleList_Read_SwitchStatus - (0x004C436E + sizeof(DWORD));
	*(LPBYTE )0x004C4372 = NOP;

	// TSSDoubleList::Write
	*(LPBYTE )0x004C5456 = JMP_REL32;
	*(LPDWORD)0x004C5457 = (DWORD)TSSDoubleList_Write_SwitchStatus - (0x004C5457 + sizeof(DWORD));
	*(LPBYTE )0x004C545B = NOP;

	// TSSDoubleList::ToByteCode
	*(LPBYTE )0x004C6734 = JMP_REL32;
	*(LPDWORD)0x004C6735 = (DWORD)TSSDoubleList_ToByteCode_SwitchStatus - (0x004C6735 + sizeof(DWORD));
	*(LPBYTE )0x004C6739 = NOP;

	// TSSDoubleToggle::Setting
	*(LPBYTE )0x004C7F03 = JMP_REL32;
	*(LPDWORD)0x004C7F04 = 0x004C827A - (0x004C7F04 + sizeof(DWORD));

	// TSSDoubleToggle::Read
	*(LPBYTE )0x004C9930 = JMP_REL32;
	*(LPDWORD)0x004C9931 = (DWORD)TSSDoubleToggle_Read_SwitchStatus - (0x004C9931 + sizeof(DWORD));
	*(LPBYTE )0x004C9935 = NOP;

	// TSSDoubleToggle::Write
	*(LPBYTE )0x004CC1DA = JMP_REL32;
	*(LPDWORD)0x004CC1DB = (DWORD)TSSDoubleToggle_Write_SwitchStatus - (0x004CC1DB + sizeof(DWORD));
	*(LPBYTE )0x004CC1DF = NOP;

	// TSSGCtrl::GetSSGDataFile
	*(LPDWORD)(0x004EE307 + 2) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE307 + 2 + sizeof(DWORD));
	*(LPDWORD)(0x004EE585 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE585 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004EE780 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE780 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004EEC13 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EEC13 + 1 + sizeof(DWORD));

	// TSSGCtrl::SetSSGDataFile
	*(LPBYTE )0x004F0A48 = JMP_REL32;
	*(LPDWORD)0x004F0A49 = (DWORD)TSSGCtrl_SetSSGDataFile_IgnoreEmptyData - (0x004F0A49 + sizeof(DWORD));
	*(LPDWORD)0x004F0A4D = NOP_X4;
}
