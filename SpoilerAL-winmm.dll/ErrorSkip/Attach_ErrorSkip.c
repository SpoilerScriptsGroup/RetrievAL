#include <windows.h>
#include "TSSGSubject.h"

EXTERN_C void __cdecl TSSBundleList_Write_ErrorSkip();
EXTERN_C void __cdecl TSSDir_WriteChildren_ErrorSkip();
EXTERN_C void __cdecl TSSGCtrl_Write_ErrorSkip();
EXTERN_C void __cdecl TSSGActionListner_OnSubjectWriteError_ErrorSkip();

#ifndef _M_IX86
typedef BOOLEAN(__cdecl * const IsSameSubject)(TSSGSubject* const this, TSSGSubject* SSGS);
#else
__declspec(naked)
#endif
static BOOLEAN __cdecl TSSDir_IsSameChildren_IsSameSubject(TSSGSubject* const this, TSSGSubject* FirstSubject) {
	extern BOOL ExtensionTSSDir;
#define stSPLIT    16
#ifndef _M_IX86
	if (ExtensionTSSDir && this->type == stSPLIT) return TRUE;
	return ((IsSameSubject)this->VTable[10])(this, FirstSubject);// tail call
#else
	__asm {
		xor   eax, eax
		mov   ecx, [esp + 4]
		cmp   byte ptr [ecx + 5], stSPLIT// TSSGSubject*->type
		cmove eax, ExtensionTSSDir
		test   al, al
		jnz   CONTINUE
		mov   edx, [ecx]
		jmp   dword ptr [edx + 0x28]// TSSGSubject*->IsSameSubject()

		align 16
	CONTINUE:
		ret
	}
#endif
#undef stSPLIT
}

#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8
#define JMP_REL32  (BYTE)0xE9

EXTERN_C void __cdecl Attach_ErrorSkip()
{
	// TSSBundleList::Write
	*(LPDWORD)(0x004BF899 + 1) = (DWORD)TSSBundleList_Write_ErrorSkip - (0x004BF899 + 1 + sizeof(DWORD));

	// TSSDir::IsSameChildren
	*(LPBYTE )0x004C3217 = CALL_REL32;
	*(LPDWORD)0x004C3218 = (DWORD)TSSDir_IsSameChildren_IsSameSubject - (0x004C3218 + sizeof(DWORD));

	// TSSDir::WriteChildren
	*(LPBYTE )0x004C329E = CALL_REL32;
	*(LPDWORD)0x004C329F = (DWORD)TSSDir_WriteChildren_ErrorSkip - (0x004C329F + sizeof(DWORD));
	*(LPBYTE )0x004C32A3 = NOP;

	// TSSGCtrl::Write
	*(LPBYTE )0x0051C455 = JMP_REL32;
	*(LPDWORD)0x0051C456 = (DWORD)TSSGCtrl_Write_ErrorSkip - (0x0051C456 + sizeof(DWORD));

	// TSSGActionListner::OnSubjectWriteError
	*(LPDWORD)(0x0052F1E0 + 1) = (DWORD)TSSGActionListner_OnSubjectWriteError_ErrorSkip - (0x0052F1E0 + 1 + sizeof(DWORD));
}
