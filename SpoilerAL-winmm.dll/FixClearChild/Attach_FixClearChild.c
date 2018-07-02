#include "SubjectStringTable/SubjectStringOperator.h"
#include "TSSDir.h"

unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

static void __fastcall TSSGCtrl_ChangeDirectorySubject_Event(TSSGCtrl* SSGC, TSSGSubject* SSD) {
	string* code = SubjectStringTable_GetString(&SSD->code);
	if (!string_empty(code)) {
		Parsing(SSGC, SSD, code, 6, "IsOpen", (unsigned __int64)(SSD->status & 1), 0);
	}
}

static PEXCEPTION_REGISTRATION_RECORD __declspec(naked) __fastcall TSSGCtrl_ChangeDirectorySubject_InitExceptBlockLDTC(LPCVOID info) {
	extern BOOL FixTheProcedure;
	extern const LPCVOID F005D54CC;
	__asm {
		cmp  [FixTheProcedure], 0
		jne  Dispatch
		jmp  dword ptr [F005D54CC]
	Dispatch:
		call dword ptr [F005D54CC]
		mov  edx, [ebp + 0x0C]
		mov  ecx, edi
		jmp  TSSGCtrl_ChangeDirectorySubject_Event
	}
}

#define NOP       (BYTE)0x90
#define JMP_REL32 (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixClearChild()
{
	// TSSDir::FixClearChild
	*(LPBYTE )0x004C2CCC = JMP_REL32;
	*(LPDWORD)0x004C2CCD = (DWORD)TSSDir_ClearChild - (0x004C2CCD + sizeof(DWORD));
	*(LPBYTE )0x004C2CD1 = NOP;

	// TSSGCtrl::ChangeDirectorySubject
	*(LPDWORD)0x0050297C = (DWORD)TSSGCtrl_ChangeDirectorySubject_InitExceptBlockLDTC - (0x0050297C + sizeof(DWORD));
}
