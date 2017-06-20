#include <windows.h>

EXTERN_C void __cdecl TSSBitList_Write_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleList_Write_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleToggle_Write_CheckFunnel();
EXTERN_C void __cdecl TSSFloatCalc_Write_CheckFunnel();
EXTERN_C void __cdecl TSSString_Write_CheckFunnel();
EXTERN_C void __cdecl TSSBundleFloatCalc_Write_CheckFunnel();

#define JZ_REL32   (WORD)0x840F
#define JNZ_REL32  (WORD)0x850F
#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void Attach_ForceFunnel()
{
	// TSSBitList::Write - CheckFunnel
	*(LPDWORD)0x004BB703 = JNZ_REL32;
	*(LPDWORD)0x004BB705 = (DWORD)TSSBitList_Write_CheckFunnel - (0x004BB705 + sizeof(DWORD));

	// TSSDoubleList::Write - CheckFunnel
	*(LPBYTE )0x004C627D = CALL_REL32;
	*(LPDWORD)0x004C627E = (DWORD)TSSDoubleList_Write_CheckFunnel - (0x004C627E + sizeof(DWORD));
	*(LPBYTE )0x004C6282 = NOP;

	// TSSDoubleToggle::Write - CheckFunnel
	*(LPBYTE )0x004CD908 = CALL_REL32;
	*(LPDWORD)0x004CD909 = (DWORD)TSSDoubleToggle_Write_CheckFunnel - (0x004CD909 + sizeof(DWORD));
	*(LPBYTE )0x004CD90D = NOP;

	// TSSFloatCalc::Write - CheckFunnel
	*(LPBYTE )0x004CE694 = CALL_REL32;
	*(LPDWORD)0x004CE695 = (DWORD)TSSFloatCalc_Write_CheckFunnel - (0x004CE695 + sizeof(DWORD));
	*(LPBYTE )0x004CE699 = NOP;

	// TSSString::Write - CheckFunnel
	*(LPDWORD)0x0052B558 = JZ_REL32;
	*(LPDWORD)0x0052B55A = (DWORD)TSSString_Write_CheckFunnel - (0x0052B55A + sizeof(DWORD));

	// TSSBundleFloatCalc::Write - CheckFunnel
	*(LPBYTE )0x0052EAF7 = CALL_REL32;
	*(LPDWORD)0x0052EAF8 = (DWORD)TSSBundleFloatCalc_Write_CheckFunnel - (0x0052EAF8 + sizeof(DWORD));
	*(LPBYTE )0x0052EAFC = NOP;
}
