#include <windows.h>
#include "TTitleSelectForm.h"
#include "TFindNameForm.h"

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void __cdecl Attach_FixSortTitle()
{
	// TTitleSelectForm::SortCmbBoxChange
	*(LPDWORD)0x00473D63 = (DWORD)TTitleSelectForm_SortFunc;

	// TTitleSelectForm::CnvString
	*(LPBYTE )0x00476940 = JMP_REL32;
	*(LPDWORD)0x00476941 = (DWORD)TTitleSelectForm_CnvString - (0x00476941 + sizeof(DWORD));
	*(LPBYTE )0x00476945 = NOP;

#ifdef zal20200619	// Avoid SetFocus for prevent freezing on ActivateHint when inactive.
	// TTitleSelectForm::TitleDGridMouseMove
	*(LPBYTE )0x004783EA = NOP;
	*(LPBYTE )0x004783EB = JMP_REL32;
	*(LPDWORD)0x004783EC = 0;

	// TTitleSelectForm::KeyDGridMouseMove
	*(LPBYTE )0x0047938B = NOP;
	*(LPBYTE )0x0047938C = JMP_REL32;
	*(LPDWORD)0x0047938D = 0;
#endif

	// TFindNameForm::CnvString
	*(LPBYTE )0x00486528 = JMP_REL32;
	*(LPDWORD)0x00486529 = (DWORD)TFindNameForm_CnvString - (0x00486529 + sizeof(DWORD));
	*(LPBYTE )0x0048652D = NOP;

#ifdef ForegroundTask
	// TApplication::ActivateHint
	*(LPBYTE )0x0057222F = NOP;
	*(LPBYTE )0x00572230 = JMP_REL32;
	*(LPDWORD)0x00572231 = 0;
#endif
}
