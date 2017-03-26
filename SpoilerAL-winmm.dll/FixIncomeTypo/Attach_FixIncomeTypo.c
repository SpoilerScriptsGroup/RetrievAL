#include <windows.h>

EXTERN_C void __cdecl TMemorySettingForm_ctor();
EXTERN_C void __cdecl TProcessAddForm_ctor();
EXTERN_C void __cdecl TGetSearchRangeForm_ctor();

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void Attach_FixIncomeTypo()
{
	// TMemorySettingForm::TMemorySettingForm
	*(LPBYTE )0x0047B47A = JMP_REL32;
	*(LPDWORD)0x0047B47B = (DWORD)TMemorySettingForm_ctor - (0x0047B47B + sizeof(DWORD));

	// TProcessAddForm::TProcessAddForm
	*(LPBYTE )0x00486C89 = JMP_REL32;
	*(LPDWORD)0x00486C8A = (DWORD)TProcessAddForm_ctor - (0x00486C8A + sizeof(DWORD));
	*(LPBYTE )0x00486C8E = NOP;

	// TGetSearchRangeForm::TGetSearchRangeForm
	*(LPBYTE )0x0048B1B6 = JMP_REL32;
	*(LPDWORD)0x0048B1B7 = (DWORD)TGetSearchRangeForm_ctor - (0x0048B1B7 + sizeof(DWORD));
}
