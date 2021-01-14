#include <windows.h>
#include "TSSGCtrl.h"

EXTERN_C void __cdecl OnSSGCtrlCleared();
EXTERN_C void __cdecl OnProcessDetach(TProcessCtrl *proc);

static TSSGAttributeSelector* __stdcall TSSGCtrl_Clear_attributeSelector(
	TSSGCtrl                     *const SSGC,
	struct _TFunctionTimer const *const funcTimer,
	MMRESULT               const *const ID)
{
	TSSGCtrl_LockClear(SSGC);
	OnProcessDetach(&SSGC->processCtrl);
	TSSDir_ClearChild(SSGC->rootSubject);
	SSGC->rootSubject->super.attribute = NULL;
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

	*(LPBYTE )0x004E43C5 = JMP_REL32;
	*(LPDWORD)0x004E43C6 = 0;// TSSDir::ClearChild

	*(LPDWORD)0x004E43FC = (DWORD)OnSSGCtrlCleared - (0x004E43FC + sizeof(DWORD));
}
