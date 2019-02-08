#include <windows.h>
#include "bcb6_operator.h"
#include "TSSGCtrl.h"

EXTERN_C BOOL ExtensionTSSDir;
EXTERN_C void __cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(void *, void *);
EXTERN_C void __cdecl TSSGAttributeSelector_MakeNowAttributeVec_push_back(void *, void *);
EXTERN_C void * __cdecl TSSGCtrl_ReadSSG_PushElement(void *, void *);

static __declspec(naked) void __cdecl TSSGAttributeSelector_StartElementCheck_new_attributeSetMap()
{
	__asm {
#define attributeSetMap (esi + 0x20)
		mov eax, ExtensionTSSDir
		neg eax
		and eax, dword ptr [attributeSetMap]
		jz  NEW
		mov dword ptr [esp], 0x004D2E28
		ret 4// goto nowAttributeList = new list<TSSGAttributeElement *>;

		align 16
	NEW:
		mov ecx, dword ptr [esp + 4]
		jmp bcb6_operator_new
#undef attributeSetMap
	}
}

static __declspec(naked) BOOLEAN __cdecl THeapAdjustmentAttribute_IsEqual_adaptScope() {
	__asm {
		cmp dword ptr [ebx + 4], atSCOPE
		jne CONTINUE
		mov dword ptr [esp], 0x00523643
	CONTINUE:
		mov eax, 1
		ret
	}
}

#define CALL_REL  (BYTE )0xE8
#define JMP_REL32 (BYTE )0xE9
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void __cdecl Attach_EnumReadSSG()
{
	// TSSGAttributeSelector::StartElementCheck
	//   attributeSetMap!=NULL => nowAttributeList!=NULL
	*(LPBYTE )(0x004D2D6D + 2) = offsetof(TSSGAttributeSelector, nowAttributeList);
	*(LPDWORD)(0x004D2D85 + 1) = (DWORD)TSSGAttributeSelector_StartElementCheck_new_attributeSetMap - (0x004D2D85 + 1 + sizeof(DWORD));

	// TSSGAttributeSelector::EndElementCheck
	//   retain attributeSetMap
	*(LPWORD )0x004D3686 = BSWAP16(0x6690);
	*(LPDWORD)0x004D3688 = BSWAP32(0x33C08943);
	*(LPDWORD)0x004D368C = BSWAP32(0x28817D04);
	*(LPDWORD)0x004D3690 = 0x004D2D15;
	*(LPBYTE )0x004D3694 = 0x74;
	*(LPBYTE )0x004D3695 = 0x004D36A2 - (0x004D3695 + sizeof(BYTE));
	*(LPWORD )0x004D3696 = BSWAP16(0x3905);
	*(LPDWORD)0x004D3698 = (DWORD)&ExtensionTSSDir;
	*(LPWORD )0x004D369C = BSWAP16(0x0F85);
	*(LPDWORD)0x004D369E = 0x004D3722 - (0x004D369E + sizeof(DWORD));

	// TSSGAttributeSelector::AddElement
	*(LPDWORD)0x004D3DCA = (DWORD)TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute - (0x004D3DCA + sizeof(DWORD));

	// TSSGAttributeSelector::MakeNowAttributeVec
	*(LPDWORD)0x004D5C88 = (DWORD)TSSGAttributeSelector_MakeNowAttributeVec_push_back - (0x004D5C88 + sizeof(DWORD));

	// TSSGCtrl::ReadSSG
	*(LPDWORD)0x004E462A = (DWORD)TSSGCtrl_ReadSSG_PushElement - (0x004E462A + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG
	*(LPBYTE )0x004E5090 = JMP_REL32;
	*(LPDWORD)0x004E5091 = (DWORD)TSSGCtrl_EnumReadSSG - (0x004E5091 + sizeof(DWORD));
	*(LPDWORD)0x004E5095 = NOP_X4;

	// TSSGCtrl::MakeSubjectClass
	*(LPDWORD)(0x004EC409 + 1) = (DWORD)TSSGCtrl_MakeSubjectType - (0x004EC409 + 1 + sizeof(DWORD));

	// TSSGCtrl::ReadADJFile
	//   attributeSelector.IsElementCheckMode()
	*(LPBYTE )(0x004FD49A + 2) = offsetof(TSSGCtrl, attributeSelector.nowAttributeList);

	// THeapAdjustmentAttribute::IsEqual
	*(LPBYTE )0x005235C8 = 0xC0;
	*(LPBYTE )0x005235CB = CALL_REL;
	*(LPDWORD)0x005235CC = (DWORD)THeapAdjustmentAttribute_IsEqual_adaptScope - (0x005235CC + sizeof(DWORD));
	*(LPBYTE )0x005235D1 = 0xC0;
}
