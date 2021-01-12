#include <windows.h>
#include "TProcessCtrl.h"

EXTERN_C void __cdecl AddEntryModule();

#define CALL_REL32 (BYTE)0xE8
#define JMP_REL32  (BYTE)0xE9
#define NOP        (BYTE)0x90

EXTERN_C void __cdecl Attach_AddEntryModule()
{
	// TProcessCtrl::LoadModuleList
#if 1
	*(LPBYTE )0x004A4E4C = JMP_REL32;
	*(LPDWORD)0x004A4E4D = (DWORD)TProcessCtrl_LoadModuleList - (0x004A4E4D + sizeof(DWORD));
#else
	*(LPBYTE )0x004A50D7 = CALL_REL32;
	*(LPDWORD)0x004A50D8 = (DWORD)AddEntryModule - (0x004A50D8 + sizeof(DWORD));
	*(LPBYTE )0x004A50DC = NOP;
#endif
}
