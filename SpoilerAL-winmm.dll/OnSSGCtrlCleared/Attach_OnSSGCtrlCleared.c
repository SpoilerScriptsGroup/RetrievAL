#include <windows.h>
#include "TSSGCtrl.h"

EXTERN_C void __cdecl OnSSGCtrlCleared();
void __cdecl OnProcessDetach();

static TSSGAttributeSelector* __stdcall TSSGCtrl_Clear_attributeSelector(
	TSSGCtrl *const SSGC,
	struct _TFunctionTimer *const funcTimer,
	MMRESULT *const ID)
{
	OnProcessDetach();
	return &SSGC->attributeSelector;
}

#define PUSH_ESI  (BYTE)0x56
#define CALL_REL  (BYTE)0xE8
#define JMP_REL32 (BYTE)0xE9

EXTERN_C void __cdecl Attach_OnSSGCtrlCleared()
{
	// TSSGCtrl::Clear
	*(LPBYTE )0x004E43B1 = PUSH_ESI;
	*(LPBYTE )0x004E43B2 = CALL_REL;
	*(LPDWORD)0x004E43B3 = (DWORD)TSSGCtrl_Clear_attributeSelector - (0x004E43B3 + sizeof(DWORD));

	*(LPBYTE )0x004E4403 = JMP_REL32;
	*(LPDWORD)0x004E4404 = (DWORD)OnSSGCtrlCleared - (0x004E4404 + sizeof(DWORD));
}
