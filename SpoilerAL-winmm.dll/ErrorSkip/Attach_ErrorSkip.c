#include <windows.h>

EXTERN_C void __cdecl TSSBundleList_Write_ErrorSkip();
EXTERN_C void __cdecl TSSDir_WriteChildren_ErrorSkip();
EXTERN_C void __cdecl TSSGCtrl_Write_ErrorSkip();
EXTERN_C void __cdecl TSSGActionListner_OnSubjectWriteError_ErrorSkip();

#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8
#define JMP_REL32  (BYTE)0xE9

EXTERN_C void Attach_ErrorSkip()
{
	// TSSBundleList::Write
	*(LPDWORD)(0x004BF899 + 1) = (DWORD)TSSBundleList_Write_ErrorSkip - (0x004BF899 + 1 + sizeof(DWORD));

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
