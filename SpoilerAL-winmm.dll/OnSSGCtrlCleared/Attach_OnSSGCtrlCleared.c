#include <windows.h>

EXTERN_C void __cdecl Caller_OnSSGCtrlCleared();

#define JMP_REL32 (BYTE)0xE9

EXTERN_C void Attach_OnSSGCtrlCleared()
{
	// TSSGCtrl::Clear
	*(LPBYTE )0x004E4403 = JMP_REL32;
	*(LPDWORD)0x004E4404 = (DWORD)Caller_OnSSGCtrlCleared - (0x004E4404 + sizeof(DWORD));
}
