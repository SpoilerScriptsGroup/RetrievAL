#include <windows.h>
#include "TSSGCtrl.h"

EXTERN_C void __cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(void *, void *);
EXTERN_C void __cdecl TSSGAttributeSelector_MakeNowAttributeVec_push_back(void *, void *);
EXTERN_C void * __cdecl TSSGCtrl_ReadSSG_PushElement(void *, void *);

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

	// THeapAdjustmentAttribute::IsEqual
	*(LPBYTE )0x005235C8 = 0xC0;
	*(LPBYTE )0x005235CB = CALL_REL;
	*(LPDWORD)0x005235CC = (DWORD)THeapAdjustmentAttribute_IsEqual_adaptScope - (0x005235CC + sizeof(DWORD));
	*(LPBYTE )0x005235D1 = 0xC0;
}
