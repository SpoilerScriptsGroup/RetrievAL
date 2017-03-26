#include <windows.h>

EXTERN_C void __cdecl Caller_AddEntryModule();

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void Attach_AddEntryModule()
{
	// TProcessCtrl::LoadModuleList
	*(LPBYTE )0x004A50D7 = JMP_REL32;
	*(LPDWORD)0x004A50D8 = (DWORD)Caller_AddEntryModule - (0x004A50D8 + sizeof(DWORD));
	*(LPBYTE )0x004A50DC = NOP;
}
