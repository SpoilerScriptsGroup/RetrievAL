#include <windows.h>

EXTERN_C void __cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(void *, void *);
EXTERN_C void __cdecl allAtteributeVecList_push_back(void *, void *);
EXTERN_C void * __cdecl rootAttributeHook(void *, void *);
EXTERN_C void __cdecl AdditionalTags();

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void __cdecl Attach_AdditionalTags()
{
	// TSSGAttributeSelector::AddElement
	*(LPDWORD)0x004D3DCA = (DWORD)TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute - (0x004D3DCA + sizeof(DWORD));

	// TSSGAttributeSelector::MakeNowAttributeVec
	*(LPDWORD)0x004D5C88 = (DWORD)allAtteributeVecList_push_back - (0x004D5C88 + sizeof(DWORD));

	// TSSGCtrl::ReadSSG
	*(LPDWORD)0x004E462A = (DWORD)rootAttributeHook - (0x004E462A + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG
	*(LPBYTE )0x004EB4FA = 0x26;
	*(LPBYTE )0x004EB4FB = JMP_REL32;
	*(LPDWORD)0x004EB4FC = (DWORD)AdditionalTags - (0x004EB4FC + sizeof(DWORD));
	*(LPBYTE )0x004EB500 = NOP;
	*(LPBYTE )0x004EB501 = NOP;
}
