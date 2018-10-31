#include <windows.h>
#include "TStringDivision.h"

EXTERN_C void __cdecl TSSCalc_Setting_CheckSignedSize();
EXTERN_C void __cdecl TSSCalc_Setting_MinMax();
EXTERN_C void __cdecl TSSBundleCalc_Setting_MinMax();
EXTERN_C void __cdecl TSSFloatCalc_Setting_MinMax();
EXTERN_C void __cdecl TSSBundleFloatCalc_Setting_MinMax();

#define NOP        (BYTE )0x90
#define NOP_X4     (DWORD)0x90909090
#define CALL_REL32 (BYTE )0xE8
#define JMP_REL32  (BYTE )0xE9

EXTERN_C void __cdecl Attach_MinMaxParam()
{
	// TSSBundleCalc::Setting
	*(LPBYTE )0x004BD111 = JMP_REL32;
	*(LPDWORD)0x004BD112 = (DWORD)TSSCalc_Setting_CheckSignedSize - (0x004BD112 + sizeof(DWORD));
	*(LPDWORD)0x004BD116 = NOP_X4;
	*(LPBYTE )0x004BD11A = NOP;

	// TSSBundleCalc::Setting
	*(LPBYTE )0x004BD1D3 = CALL_REL32;
	*(LPDWORD)0x004BD1D4 = (DWORD)TSSBundleCalc_Setting_MinMax - (0x004BD1D4 + sizeof(DWORD));

	// TSSCalc::Setting
	*(LPBYTE )0x004C189F = JMP_REL32;
	*(LPDWORD)0x004C18A0 = (DWORD)TSSCalc_Setting_CheckSignedSize - (0x004C18A0 + sizeof(DWORD));
	*(LPDWORD)0x004C18A4 = NOP_X4;
	*(LPBYTE )0x004C18A5 = NOP;

	// TSSCalc::Setting
	*(LPBYTE )0x004C195F = CALL_REL32;
	*(LPDWORD)0x004C1960 = (DWORD)TSSCalc_Setting_MinMax - (0x004C1960 + sizeof(DWORD));

	// TSSFloatCalc::Setting
	*(LPBYTE )0x004CDE7D = CALL_REL32;
	*(LPDWORD)0x004CDE7E = (DWORD)TSSFloatCalc_Setting_MinMax - (0x004CDE7E + sizeof(DWORD));

	// TSSBundleFloatCalc::Setting
	*(LPBYTE )0x0052D80D = CALL_REL32;
	*(LPDWORD)0x0052D80E = (DWORD)TSSBundleFloatCalc_Setting_MinMax - (0x0052D80E + sizeof(DWORD));
}
