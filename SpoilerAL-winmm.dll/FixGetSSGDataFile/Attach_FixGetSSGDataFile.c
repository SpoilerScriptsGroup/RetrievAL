#include <windows.h>
#include "TStringDivision.h"
#include "TSSGSubject.h"

EXTERN_C void __cdecl FixGetSSGDataFile();
EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);

static __declspec(naked) void TSSGCtrl_GetSSGDataFile_Half(
	bcb6_std_string *FileName,
	TStringDivision *strD,
	bcb6_std_string *FName,
	bcb6_std_string  Token,
	unsigned long    Index,
	unsigned long    Option) {
	extern BOOL EnableParserFix;
	__asm {
		cmp  EnableParserFix, 0
		je   SKIP
		mov  eax, [esp + 0x0C]// FName
		mov  edx, [ebp + 0x0C]// SSGS
		test edx, edx
		jz   SKIP
		push eax
		push edx
		call ReplaceDefineDynamic
	SKIP:
		jmp  TStringDivision_Half
	}
}

EXTERN_C void __cdecl Attach_FixGetSSGDataFile()
{
	// TSSGCtrl::GetSSGDataFile
	*(LPDWORD)(0x004EF8E9 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_Half - (0x004EF8E9 + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004EF9C2 + 1) = (DWORD)FixGetSSGDataFile - (0x004EF9C2 + 1 + sizeof(DWORD));
}
