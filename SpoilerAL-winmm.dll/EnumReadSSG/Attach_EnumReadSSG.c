#include <windows.h>
#include "TSSGCtrl.h"

EXTERN_C void __cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(void *, void *);
EXTERN_C void __cdecl allAtteributeVecList_push_back(void *, void *);
EXTERN_C void * __cdecl rootAttributeHook(void *, void *);

#define JMP_REL32 (BYTE )0xE9
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void __cdecl Attach_EnumReadSSG()
{
	// TSSGAttributeSelector::AddElement
	*(LPDWORD)0x004D3DCA = (DWORD)TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute - (0x004D3DCA + sizeof(DWORD));

	// TSSGAttributeSelector::MakeNowAttributeVec
	*(LPDWORD)0x004D5C88 = (DWORD)allAtteributeVecList_push_back - (0x004D5C88 + sizeof(DWORD));

	// TSSGCtrl::ReadSSG
	*(LPDWORD)0x004E462A = (DWORD)rootAttributeHook - (0x004E462A + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG
	*(LPBYTE )0x004E5090 = JMP_REL32;
	*(LPDWORD)0x004E5091 = (DWORD)TSSGCtrl_EnumReadSSG - (0x004E5091 + sizeof(DWORD));
	*(LPDWORD)0x004E5095 = NOP_X4;

	// TSSGCtrl::MakeSubjectClass
	*(LPDWORD)(0x004EC409 + 1) = (DWORD)TSSGCtrl_MakeSubjectType - (0x004EC409 + 1 + sizeof(DWORD));
}
